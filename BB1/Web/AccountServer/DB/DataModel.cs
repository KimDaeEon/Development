using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Threading.Tasks;

namespace AccountServer.DB
{
    // TODO: EF Core 에서 pkey 를 기본적으로 Identity 를 써서 하는데, 
    // 이렇게 해버리면 DB 샤딩이 어려워서 나중에 서버 합칠 때에 어렵고, Sequence 를 쓰는게 속도적으로도 더 빠르다.
    // 당장은 필요없을 것 같은데 Sequence 적용하는 것도 EF 와 함께하는 방법을 고민해보자.
    [Table("Account")]
    public class AccountDb
    {
        public int AccountDbId { get; set; }
        public string AccountName { get; set; }
        public string Password { get; set; }
    }
}
