using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

// ���� Ŭ���̾�Ʈ�� �̷� ������ UI ���� �ڵ� 1��, �̰Ϳ� �����Ǵ� ���� ���� �ڵ� 1���� ��ȣ�ۿ��Ѵٰ� ����ȴ�.
public class UI_SelectServerPopup : UI_Popup
{
    public List<UI_SelectServerPopup_Item> Items { get; } = new List<UI_SelectServerPopup_Item>();

    public override void Init()
    {
        base.Init();
    }

    public void SetServerInfos(List<ServerInfo> servers)
    {
        Items.Clear();

        GameObject itemGrid = GetComponentInChildren<GridLayoutGroup>().gameObject;
        foreach (Transform child in itemGrid.transform)
        {
            Destroy(child.gameObject);
        }

        foreach(var serverInfo in servers)
        {
            GameObject go = Managers.Resource.Instantiate("UI/Popup/UI_SelectServerPopup_Item", itemGrid.transform);
            UI_SelectServerPopup_Item item = go.GetOrAddComponent<UI_SelectServerPopup_Item>();
            item.Info = serverInfo;
            Items.Add(item);
        }
    }

    public void RefreshUI()
    {
        if(Items.Count == 0)
        {
            return;
        }

        foreach(var item in Items)
        {
            item.RefreshUI();
        }
    }
}
