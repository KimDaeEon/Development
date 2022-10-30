using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace SharedDB
{
    // 인증을 받고나서 게임 서버에 접속하는 절차를 위해 사용되는 토큰
    [Table("Token")]
    public class TokenDb
    {
        public int TokenDbId { get; set; }
        public int AcountDbId { get; set; }
        public int Token { get; set; }
        public DateTime ExpiredDatetime { get; set; }
    }

    [Table("ServerInfo")]
    public class ServerDb
    {
        public int ServerDbId { get; set; }
        public string Name { get; set; }
        public string IpAddress { get; set; }
        public int Port { get; set; }
        public int CrowdedLevel { get; set; }
    }
}
