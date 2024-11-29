using Data;
using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

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



    public static Item MakeItem(ItemInfo itemInfo)
    {
        Item item = null;

        ItemData itemData;
        Managers.Data.ItemDict.TryGetValue(itemInfo.TemplateId, out itemData);

        if (itemData == null)
        {
            UnityEngine.Debug.Log($"Something is wrong with item templatedId : {itemInfo.TemplateId}");
            return item;
        }

        switch (itemData.itemType)
        {
            case ItemType.Weapon:
                item = new WeaponItem(itemInfo.TemplateId);
                break;

            case ItemType.Armor:
                item = new ArmorItem(itemInfo.TemplateId);
                break;

            case ItemType.Consumable:
                item = new ConsumableItem(itemInfo.TemplateId);
                break;

            default:
                UnityEngine.Debug.Log("Something is wrong with item templatedId : {itemInfo.TemplateId}");
                break;
        }

        if (item != null)
        {
            item.ItemDbId = itemInfo.ItemDbId;
            item.Count = itemInfo.Count;
            item.SlotNum = itemInfo.SlotNum;
            item.Equipped = itemInfo.Equipped;
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
        Managers.Data.ItemDict.TryGetValue(templateId, out itemData);

        if (itemData.itemType != ItemType.Weapon)
        {
            System.Diagnostics.Debug.Assert(false); // TODO: 이런 데이터 문제는 크리티컬 로그 찍고 서버 바로 내려가게 하자.
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
        Managers.Data.ItemDict.TryGetValue(templateId, out itemData);

        if (itemData.itemType != ItemType.Armor)
        {
            System.Diagnostics.Debug.Assert(false); // TODO: 이런 데이터 문제는 크리티컬 로그 찍고 서버 바로 내려가게 하자.
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
        Managers.Data.ItemDict.TryGetValue(templateId, out itemData);

        if (itemData.itemType != ItemType.Consumable)
        {
            System.Diagnostics.Debug.Assert(false); // TODO: 이런 데이터 문제는 크리티컬 로그 찍고 서버 바로 내려가게 하자.
        }

        ConsumableData data = (ConsumableData)itemData;
        TemplateId = data.templateId;
        Count = 1;
        MaxCount = data.maxCount;
        ConsumableType = data.consumableType;
        Stackable = (data.maxCount > 1); // 보통은 소모 아이템이면 이게 1보다 크겠지만..
    }
}