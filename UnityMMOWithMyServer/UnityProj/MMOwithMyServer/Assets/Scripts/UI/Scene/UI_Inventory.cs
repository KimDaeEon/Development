using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

// 보통 클라이언트는 이런 식으로 UI 관리 코드 1개, 이것에 대응되는 로직 관련 코드 1개가 상호작용한다고 보면된다.
public class UI_Inventory : UI_Base
{
    public List<UI_Inventory_Item> Items { get; } = new List<UI_Inventory_Item>();

    public override void Init()
    {
        Items.Clear();

        GameObject itemGrid = transform.Find("ItemGrid").gameObject;
        foreach(Transform child in itemGrid.transform)
        {
            Destroy(child.gameObject);
        }

        for (int i = 0; i < 20; i++)
        {
            GameObject go = Managers.Resource.Instantiate("UI/Scene/UI_Inventory_Item", itemGrid.transform);
            UI_Inventory_Item uiInvenItem = go.GetOrAddComponent<UI_Inventory_Item>();
            Items.Add(uiInvenItem); // UI 용 Item 정보 등록
        }

        RefreshUI();
    }

    public void RefreshUI()
    {
        if(Items.Count == 0)
        {
            return;
        }

        List<Item> items = Managers.ItemInventory.Items.Values.ToList();
        items.Sort((left, right) => { return left.Info.SlotNum - right.SlotNum; });

        foreach (Item item in items)
        {
            if (item.SlotNum < 0 && item.SlotNum > 20)
            {
                continue;
            }

            Items[item.SlotNum].SetItem(item);
        }
    }
}
