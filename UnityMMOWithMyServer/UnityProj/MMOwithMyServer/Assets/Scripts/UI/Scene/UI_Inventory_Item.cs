using Google.Protobuf.Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_Inventory_Item : UI_Base
{
    [SerializeField]
    Image _icon = null;

    [SerializeField]
    Image _frame = null;

    public int ItemDbId { get; private set; }
    public int TemplatedId { get; private set; }
    public int Count { get; private set; }
    public bool Equipped { get; private set; }

    public override void Init()
    {
        _icon.gameObject.BindEvent((e) =>
        {
            Debug.Log("Item Equip");

            Data.ItemData itemData;
            Managers.Data.ItemDict.TryGetValue(TemplatedId, out itemData);

            // TODO: 포션 같은 경우는 다른 패킷으로 처리
            if(itemData.itemType == ItemType.Consumable)
            {
                return;
            }

            C_EquipItem equipItemPacket = new C_EquipItem();
            equipItemPacket.ItemDbId = ItemDbId;
            equipItemPacket.Equipped = !Equipped;

            Managers.Network.Send(equipItemPacket);
        });
    }

    public void SetItem(Item item)
    {
        if(item == null)
        {
            ItemDbId = 0;
            TemplatedId = 0;
            Count = 0;
            Equipped = false;

            _icon.gameObject.SetActive(false);
            _frame.gameObject.SetActive(false);
        }
        else
        {
            ItemDbId = item.ItemDbId;
            TemplatedId = item.TemplateId;
            Count = item.Count;
            Equipped = item.Equipped;

            Data.ItemData itemData;
            Managers.Data.ItemDict.TryGetValue(item.TemplateId, out itemData);

            Sprite icon = Managers.Resource.Load<Sprite>(itemData.iconPath);
            _icon.sprite = icon;

            _icon.gameObject.SetActive(true);
            _frame.gameObject.SetActive(item.Equipped);
        }
    }
}
