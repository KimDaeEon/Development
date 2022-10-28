using CSharpServer.Data;
using CSharpServer.DB;
using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    // TODO: 여기에 그냥 ItemData 포인터를 들고 있게 해도 될 것 같은데.. 고민 필요
    // ItemInfo Info 를 들고 있고, 패킷 만들 때에 편하게 하는 것 + Dict 를 수정하는 human error 방지용인 듯 하다.
    public class Item
    {
        public ItemInfo Info { get; } = new ItemInfo();

        public int ItemDbId
        {
            get { return Info.ItemDbId; }
            set { Info.ItemDbId = value; }
        }

        public int TemplateId
        {
            get { return Info.TemplateId; }
            set { Info.TemplateId = value; }
        }

        public int Count
        {
            get { return Info.Count; }
            set { Info.Count = value; }
        }
        public int SlotNum
        {
            get { return Info.SlotNum; }
            set { Info.SlotNum = value; }
        }

        public bool Equipped
        {
            get { return Info.Equipped; }
            set { Info.Equipped = value; }
        }

        public ItemType ItemType { get; private set; }
        public bool Stackable { get; protected set; }
        
        public Item(ItemType itemType)
        {
            ItemType = itemType;
        }



        public static Item MakeItem(ItemDb itemDb)
        {
            Item item = null;

            ItemData itemData;
            DataManager.ItemDict.TryGetValue(itemDb.TemplateId, out itemData);

            if (itemData == null)
            {
                Console.WriteLine($"Something is wrong with item templatedId : {itemDb.TemplateId}");
                return item;
            }

            switch (itemData.itemType)
            {
                case ItemType.Weapon:
                    item = new WeaponItem(itemDb.TemplateId);
                    break;

                case ItemType.Armor:
                    item = new ArmorItem(itemDb.TemplateId);
                    break;

                case ItemType.Consumable:
                    item = new ConsumableItem(itemDb.TemplateId);
                    break;

                default:
                    Console.WriteLine($"Something is wrong with item templatedId : {itemDb.TemplateId}");
                    break;
            }

            if (item != null)
            {
                item.ItemDbId = itemDb.ItemDbId;
                item.Count = itemDb.Count;
                item.SlotNum = itemDb.SlotNum;
                item.Equipped = itemDb.Equipped;
            }

            return item;
        }
    }

    public class WeaponItem : Item
    {
        public WeaponType WeaponType { get; private set; }
        public int Damage { get; private set; }

        public WeaponItem(int templateId) : base(ItemType.Weapon)
        {
            Init(templateId);
        }

        void Init(int templateId)
        {
            ItemData itemData;
            DataManager.ItemDict.TryGetValue(templateId, out itemData);

            if(itemData.itemType != ItemType.Weapon)
            {
                Debug.Assert(false); // TODO: 이런 데이터 문제는 크리티컬 로그 찍고 서버 바로 내려가게 하자.
            }

            WeaponData data = (WeaponData)itemData;
            TemplateId = data.templateId;
            Count = 1;
            WeaponType = data.weaponType;
            Damage = data.damage;
            Stackable = false;
        }
    }

    public class ArmorItem : Item
    {
        public ArmorType ArmorType { get; private set; }
        public int Defence { get; private set; }

        public ArmorItem(int templateId) : base(ItemType.Armor)
        {
            Init(templateId);
        }

        void Init(int templateId)
        {
            ItemData itemData;
            DataManager.ItemDict.TryGetValue(templateId, out itemData);

            if (itemData.itemType != ItemType.Armor)
            {
                Debug.Assert(false); // TODO: 이런 데이터 문제는 크리티컬 로그 찍고 서버 바로 내려가게 하자.
            }

            ArmorData data = (ArmorData)itemData;
            TemplateId = data.templateId;
            Count = 1;
            ArmorType = data.armorType;
            Defence = data.defence;
            Stackable = false;
        }
    }

    public class ConsumableItem : Item
    {
        public ConsumableType ConsumableType { get; private set; }
        public int MaxCount { get; private set; }

        public ConsumableItem(int templateId) : base(ItemType.Consumable)
        {
            Init(templateId);
        }

        void Init(int templateId)
        {
            ItemData itemData;
            DataManager.ItemDict.TryGetValue(templateId, out itemData);

            if (itemData.itemType != ItemType.Consumable)
            {
                Debug.Assert(false); // TODO: 이런 데이터 문제는 크리티컬 로그 찍고 서버 바로 내려가게 하자.
            }

            ConsumableData data = (ConsumableData)itemData;
            TemplateId = data.templateId;
            Count = 1;
            MaxCount = data.maxCount;
            ConsumableType = data.consumableType;
            Stackable = (data.maxCount > 1); // 보통은 소모 아이템이면 이게 1보다 크겠지만..
        }
    }
}
