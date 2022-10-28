using CSharpServer.DB;
using CSharpServer.Utils;
using Google.Protobuf.Protocol;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class Player : Actor
    {
        public int PlayerDbId { get; set; }
        public ClientSession Session { get; set; }
        public VisionArea Vision { get; private set; }
        public Inventory Inven { get; private set; } = new Inventory();

        // TODO: 후에 추가 스탯 같은 것이 많아지면 어떻게 관리할지 고민이 필요
        // 캐릭터 기본 능력치에 더해진 아이템 스텟 정보
        public int WeaponDamage { get; private set; }
        public int ArmorDefence { get; private set; }
        public override int TotalAttack { get { return Stat.Attack + WeaponDamage; } }
        public override int TotalDeffence { get { return ArmorDefence; } }

        public Player()
        {
            Type = ActorType.Player;
            Vision = new VisionArea(this);
        }

        public static bool IsInValidSessionAndPlayerAndRoom(ClientSession session, ref Player player, ref Room room)
        {
            if (null == session)
            {
                return true;
            }

            player = session.CurrentPlayer;
            if (null == player)
            {
                return true;
            }

            room = player.CurrentRoom;
            if (null == room)
            {
                return true;
            }

            return false;
        }

        public override void OnDamaged(Actor attacker, int damage)
        {
            base.OnDamaged(attacker, damage);

        }

        public override void OnDead(Actor attacker)
        {
            base.OnDead(attacker);
        }

        public void OnLeaveGame()
        {
            DbJobQueue.SavePlayerStat(this, CurrentRoom);
        }

        public void HandlerEquipItem(C_EquipItem equipItemPacket)
        {
            Item item = Inven.Get(equipItemPacket.ItemDbId);
            if (item == null)
            {
                return;
            }

            if (item.ItemType == ItemType.Consumable)
            {
                return;
            }

            // 착용 요청이라면 겹치는 부위는 해제
            if (equipItemPacket.Equipped)
            {
                Item toBeUnequippedItem = null;

                // 무기인 경우는 1개만 착용
                if (item.ItemType == ItemType.Weapon)
                {
                    toBeUnequippedItem = Inven.Find(
                        i => i.Equipped &&
                        i.ItemType == ItemType.Weapon);
                }

                // 방어구인 경우 착용 부위까지 일치해야 한다.
                else if (item.ItemType == ItemType.Armor)
                {
                    ArmorType armorType = ((ArmorItem)item).ArmorType;
                    toBeUnequippedItem = Inven.Find(
                        i => i.Equipped &&
                        i.ItemType == ItemType.Armor &&
                        ((ArmorItem)i).ArmorType == armorType);
                }

                if (toBeUnequippedItem != null)
                {
                    // 아이템 해제 메모리 선 적용
                    toBeUnequippedItem.Equipped = false;

                    DbJobQueue.EquipItemNotify(this, toBeUnequippedItem);

                    S_EquipItem resEquipItemPacket = new S_EquipItem();
                    resEquipItemPacket.ItemDbId = toBeUnequippedItem.ItemDbId;
                    resEquipItemPacket.Equipped = toBeUnequippedItem.Equipped;
                    Session.Send(resEquipItemPacket);
                }
            }


            {
                // 아이템 착용 또는 해제 메모리 선 적용
                item.Equipped = equipItemPacket.Equipped;

                DbJobQueue.EquipItemNotify(this, item);

                S_EquipItem resEquipItemPacket = new S_EquipItem();
                resEquipItemPacket.ItemDbId = equipItemPacket.ItemDbId;
                resEquipItemPacket.Equipped = equipItemPacket.Equipped;
                Session.Send(resEquipItemPacket);
            }

            RefreshStat();
        }

        public void RefreshStat()
        {
            WeaponDamage = 0;
            ArmorDefence = 0;

            foreach(Item item in Inven.Items.Values)
            {
                if(item.Equipped == false)
                {
                    continue;
                }

                switch (item.ItemType)
                {
                    case ItemType.Weapon:
                        WeaponDamage += ((WeaponItem)item).Damage;
                        break;

                    case ItemType.Armor:
                        ArmorDefence += ((ArmorItem)item).Defence;
                        break;
                }
            }
        }
    }
}
