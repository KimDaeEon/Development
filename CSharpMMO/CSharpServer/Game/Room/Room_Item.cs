using CSharpServer.Data;
using CSharpServer.DB;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public partial class Room : JobQueue
    {
        // TODO: 한 번에 여러 아이템 착용하고, 그 결과로 여러 아이템에 대한 것을 전송하도록 변경필요
        public void HandleEquipItem(Player player, C_EquipItem equipItemPacket)
        {
            player.HandlerEquipItem(equipItemPacket);
        }
    }
}
