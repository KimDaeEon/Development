using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace EFCore_Practice
{
    // 아래처럼 데이터를 모델링한 클래스를 Entity 클래스 라고 한다.
    public class Item
    {
        public int ItemId { get; set; }
        public int TypeId { get; set; }
        public DateTime CreatedDate { get; set; }

        // 다른 클래스를 참조하고 있다면 Foreign key 로 참조한다. (Navigational Property)
        public Player Owner { get; set; }

        public override string ToString()
        {
            string result = $"ItemId : {ItemId}, TypeId: {TypeId}, CreateDate: {CreatedDate}, Owner: {Owner}";
            return result;
        }
    }

    //[Table("MyPlayer")] // 이렇게 해서 테이블 이름 명시적으로 설정 가능
    public class Player
    {
        public int PlayerId { get; set; } // 클래스이름+Id 로 해두면 EFCore 에서 자동으로 Primary key 로 설정해준다.
        public string Name { get; set; }
        public ICollection<Item> Items { get; set; }
        public Guild Guild { get; set; }
        public override string ToString()
        {
            string result = $"Player : {PlayerId}, Name: {Name}, Guild: {Guild}";

            return result;
        }
    }

    public class Guild
    {
        public int GuildId { get; set; }
        public string GuildName { get; set; }
        public ICollection<Player> Members { get; set; }

        public override string ToString()
        {
            string result = $"Guild: {GuildId}, GuildName: {GuildName}";
            return result;
        }
    }
}
