using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.DB
{
    // TODO: EF Core 에서 primary key 를 기본적으로 Identity 를 써서 하는데, 
    // 이렇게 해버리면 DB 샤딩이 어려워서 나중에 서버 합칠 때에 어렵고, Sequence 를 쓰는게 속도적으로도 더 빠르다.
    // 당장은 필요없을 것 같은데 Sequence 적용하는 것도 EF 와 함께하는 방법을 고민해보자.
    [Table("Account")]
    public class AccountDb
    {
        public int AccountDbId { get; set; }
        public string AccountName { get; set; }
        public ICollection<PlayerDb> Players { get; set; }
    }

    [Table("Player")]
    public class PlayerDb
    {
        public int PlayerDbId { get; set; }
        public string PlayerName { get; set; }

        [ForeignKey("Account")] // Account 라는 멤버와 짝이라는 것을 의미
        public int AccountDbId { get; set; }
        public AccountDb Account { get; set; }

        public ICollection<ItemDb> Items { get; set; }

        // Stat
        public int Level { get; set; }
        public int Hp { get; set; }
        public int MaxHp { get; set; }
        public int Attack { get; set; }
        public float Speed { get; set; }
        public int TotalExp { get; set; }
    }

    // TODO: 당장은 아이템이 적어서 TemplateId 로 다 처리되지만, Type 같은 정보 추가해서 아이템 구조 어떻게 할 지 고민 필요
    [Table("Item")]
    public class ItemDb
    {
        public int ItemDbId { get; set; }
        public int TemplateId { get; set; } // 아이템 시트 같은 곳에서 참조되는 Id
        public int Count { get; set; }
        public int SlotNum { get; set; }
        public bool Equipped { get; set; } = false; // 아이템 장착 여부

        [ForeignKey("Owner")]
        public int? OwnerDbId { get; set; }
        public PlayerDb Owner { get; set; }
    }
}
