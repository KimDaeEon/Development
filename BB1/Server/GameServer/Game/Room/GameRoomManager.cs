using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public partial class GameRoomManager : JobQueue
    {
        public static GameRoomManager Instance { get; } = new GameRoomManager();

        Dictionary<uint, GameRoom> _rooms = new Dictionary<uint, GameRoom>();
        int _roomIdIssuer = 1;

        public void Update()
        {
            Flush();

            foreach (var room in _rooms.Values)
            {
                room.Update();
            }
        }

        //public GameRoom Add(uint )
    }
}
