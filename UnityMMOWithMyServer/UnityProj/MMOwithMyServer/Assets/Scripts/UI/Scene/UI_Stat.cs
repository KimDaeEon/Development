using Data;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Google.Protobuf.Protocol;

public class UI_Stat : UI_Base
{
    enum Images
    {
        Slot_Head,
        Slot_Chest,
        Slot_Foot,
        Slot_Weapon,
    }

    enum Texts
    {
        NameText,
        AttackValueText,
        DefenceValueText
    }

    bool _init = false;
    public override void Init()
    {
        Bind<Image>(typeof(Images));
        Bind<Text>(typeof(Texts));

        _init = true;
        RefreshUI();
    }

    public void RefreshUI()
    {
        if (_init == false)
        {
            return;
        }

        Get<Image>((int)Images.Slot_Weapon).enabled = false;
        Get<Image>((int)Images.Slot_Head).enabled = false;
        Get<Image>((int)Images.Slot_Chest).enabled = false;
        Get<Image>((int)Images.Slot_Foot).enabled = false;

        // 착용한 아이템을 스탯 창에 채워준다.
        foreach (Item item in Managers.ItemInventory.Items.Values)
        {
            if(item.Equipped == false)
            {
                continue;
            }

            ItemData itemData;
            Managers.Data.ItemDict.TryGetValue(item.TemplateId, out itemData);
            Sprite icon = Managers.Resource.Load<Sprite>(itemData.iconPath);

            if(item.ItemType == ItemType.Weapon)
            {
                Get<Image>((int)Images.Slot_Weapon).enabled = true;
                Get<Image>((int)Images.Slot_Weapon).sprite = icon;
            }
            else if(item.ItemType == ItemType.Armor)
            {
                ArmorItem armorItem = (ArmorItem)item;
                switch (armorItem.ArmorType)
                {
                    case ArmorType.Head:
                        Get<Image>((int)Images.Slot_Head).enabled = true;
                        Get<Image>((int)Images.Slot_Head).sprite = icon;
                        break;

                    case ArmorType.Chest:
                        Get<Image>((int)Images.Slot_Chest).enabled = true;
                        Get<Image>((int)Images.Slot_Chest).sprite = icon;
                        break;

                    case ArmorType.Foot:
                        Get<Image>((int)Images.Slot_Foot).enabled = true;
                        Get<Image>((int)Images.Slot_Foot).sprite = icon;
                        break;
                }
            }
        }

        MyPlayerController player = Managers.Object.MyPlayer;
        player.RefreshStat();

        Get<Text>((int)Texts.NameText).text = player.name;
        Get<Text>((int)Texts.AttackValueText).text = $"{player.Stat.Attack } + {player.WeaponDamage}";
        Get<Text>((int)Texts.DefenceValueText).text = $"{player.ArmorDefence}";
    }
}
