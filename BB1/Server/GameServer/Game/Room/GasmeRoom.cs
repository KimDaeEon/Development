using Microsoft.EntityFrameworkCore.ChangeTracking;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public partial class GameRoom : JobQueue
    {
        public uint GameRoomId {  get; set; }
        public uint DataSheetId { get; set; }

        Dictionary<uint, Entity> _entites = new Dictionary<uint, Entity>();


        public void Update()
        {
            Flush();
        }
    }
}
