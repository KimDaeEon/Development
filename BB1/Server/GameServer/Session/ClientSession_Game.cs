using Microsoft.EntityFrameworkCore;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameServer.DB.EFCore;

namespace GameServer
{
    public partial class ClientSession : PacketSession
    {
        public Character MyCharacter {  get; set; } // TODO: 게임 스레드에서만 할당 및 해제가 이루어지기에, 일단 lock 안잡음
    }
}
