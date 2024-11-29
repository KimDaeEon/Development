using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

// ���� Ŭ���̾�Ʈ�� �̷� ������ UI ���� �ڵ� 1��, �̰Ϳ� �����Ǵ� ���� ���� �ڵ� 1���� ��ȣ�ۿ��Ѵٰ� ����ȴ�.
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
            Items.Add(uiInvenItem); // UI �� Item ���� ���
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
