using CSharpServer.Data;
using CSharpServer.Game;
using CSharpServer.Utils;
using Google.Protobuf.Protocol;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.DB
{
    // DB 에서 선 처리 후 메모리에 반영되어야 하는 작업들
    public partial class DbJobQueue : JobQueue
    {
        // TODO: 이 Job 들도 쪼개서 할 수 있으면 좋을 것 같다.
        public static DbJobQueue Instance { get; } = new DbJobQueue();

        public static void SavePlayerStat(Player player, Room room)
        {
            if (player == null || room == null)
            {
                return;
            }

            PlayerDb playerDb = new PlayerDb();
            playerDb.PlayerDbId = player.PlayerDbId;
            playerDb.Hp = player.Stat.Hp;

            Instance.Push(() =>
            {
                using (AppDbContext db = new AppDbContext())
                {
                    db.Entry(playerDb).State = EntityState.Unchanged;
                    db.Entry(playerDb).Property(nameof(PlayerDb.Hp)).IsModified = true;
                    bool success = db.SaveChangesEx();

                    if (success)
                    {
                        room.Push(() =>
                        {
#if DEBUG
                            Console.WriteLine($"Hp Saved {playerDb}");
#endif
                        });
                    }

                }
            }
            );

        }

        // 아래 함수는 무조건 EnterGame 후 room 이 할당되었을 때에 호출되어야 한다.
        public static void RewardPlayer(Player player, RewardData rewardData, Room room)
        {
            if(player == null || rewardData == null || room == null)
            {
                return;
            }

            // TODO: 거의 동시에 몬스터를 잡고, 먼저 Slot 점유한 작업이 메모리 반영 전에 다음 작업이 들어오면,
            // 슬롯 번호가 겹칠 수 있으므로 슬롯번호 예약해서 겹치지 않도록 하는 처리가 필요
            int? slotNum = player.Inven.GetEmptySlotNum();
            // TODO: 우편함으로 아이템 쏴주는 기능 만들어야 한다.
            if(slotNum == null)
            {
                return;
            }

            ItemDb itemDb = new ItemDb()
            {
                TemplateId = rewardData.itemTemplateId,
                Count = rewardData.count,
                SlotNum = slotNum.Value, // 이거 0으로 하면 안될 것 같은데..
                OwnerDbId = player.PlayerDbId
            };

            Instance.Push(() =>
            {
                using (AppDbContext db = new AppDbContext())
                {
                    db.Items.Add(itemDb);
                    bool success = db.SaveChangesEx();

                    if (success)
                    {
                        room.Push(() =>
                        {
                            Item newItem = Item.MakeItem(itemDb);
                            player.Inven.Add(newItem);

                            // 클라이언트에게 새로운 아이템 얻었음을 통지
                            S_AddItem addItemPacket = new S_AddItem();
                            ItemInfo itemInfo = new ItemInfo();
                            itemInfo.MergeFrom(newItem.Info);
                            addItemPacket.Items.Add(itemInfo);

                            player.Session.Send(addItemPacket);
                        });
                    }
                }
            }
            );

        }

    }
}
