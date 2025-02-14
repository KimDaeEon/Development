using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer.DB.EFCore
{
    // TODO: 아래 DB 키들 Sequence나 접두사 붙여서 저장하도록 변경, 추후 서버 통합까지 고려해서
    [Table("Character")]
    public class CharacterDb
    {
        // Convention : [클래스]Id 으로 명명하면 PK
        public int Id { get; set; }
        public long AccountDbId { get; set; }
        public DateTime CreateDate { get; private set; }
    }

    // TODO: 당장은 아이템이 적어서 DataSheetId 로 다 처리되지만, Type 같은 정보 추가해서 아이템 구조 어떻게 할 지 고민 필요
    [Table("Item")]
    public class ItemDb
    {
        // Convention : [클래스]Id 으로 명명하면 PK
        public long Id { get; set; }
        public int DataSheetId { get; set; }
        public int EquipSlot { get; set; }
        public long AccountDbId { get; set; }
        public int Count { get; set; }
        public int OwnerDbId { get; set; }
        public int EnchantCount { get; set; }
    }
}
