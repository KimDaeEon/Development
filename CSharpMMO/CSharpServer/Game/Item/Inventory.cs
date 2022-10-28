using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class Inventory
    {
        public Dictionary<int, Item> Items { get; } = new Dictionary<int, Item>();

        public void Add(Item item)
        {
            Items.Add(item.ItemDbId, item);
        }

        public Item Get(int itemDbId)
        {
            Item item;
            Items.TryGetValue(itemDbId, out item);

            return item;
        }

        public Item Find(Func<Item, bool> condition)
        {
            foreach (Item item in Items.Values)
            {
                if (condition.Invoke(item))
                {
                    return item;
                }
            }

            return null;
        }

        // TODO: 이 부분도 slot 칸 수 데이터로 다 빼서 연동되게 바꿔야 한다. 클라이언트도 수정해야 함.
        public int? GetEmptySlotNum()
        {
            for (int slotNum = 0; slotNum < 20; slotNum++)
            {
                Item item = Items.Values.FirstOrDefault(item => item.SlotNum == slotNum);
                if(item == null)
                {
                    return slotNum;
                }
            }

            return null;
        }
    }
}
