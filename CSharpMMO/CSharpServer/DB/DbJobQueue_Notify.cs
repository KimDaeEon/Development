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
    // 메모리 선 적용 후 DB 에 그냥 던져도 상관없는 작업들
    public partial class DbJobQueue : JobQueue
    {
        public static void EquipItemNotify(Player player, Item item)
        {
            if (player == null || item == null)
            {
                return;
            }

            ItemDb itemDb = new ItemDb()
            {
                ItemDbId = item.ItemDbId,
                Equipped = item.Equipped
            };

            Instance.Push(() =>
            {
                using (AppDbContext db = new AppDbContext())
                {
                    db.Entry(itemDb).State = EntityState.Unchanged;
                    db.Entry(itemDb).Property(nameof(ItemDb.Equipped)).IsModified = true;

                    bool success = db.SaveChangesEx();

                    if (success)
                    {
                    }
                    else
                    {
                        // TODO: 실패했으면 Kick
                    }
                }
            }
            );

        }
    }
}
