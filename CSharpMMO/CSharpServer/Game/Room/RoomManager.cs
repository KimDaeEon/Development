using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class RoomManager : JobQueue
    {
        public static RoomManager Instance { get; } = new RoomManager();

        Dictionary<int, Room> _rooms = new Dictionary<int, Room>();
        int _roomId = 1;

        // 아래의 모든 함수들은 Push 를 통해서 순차적으로 실행되어야만 한다.
        // 외부에서 함부로 호출되면 안된다.
        public void Update()
        {
            Flush(); // room 제거, 삽입 등이 여기서 처리된다.

            foreach (Room room in _rooms.Values)
            {
                room.Update();
            }
        }

        public Room Add(int mapId)
        {
            Room room = new Room();
            room.Push(room.Init, mapId, Room.VisionCells * 2);

            room.RoomId = _roomId;
            _rooms.Add(_roomId, room);
            _roomId++;

            return room;
        }

        public bool Remove(int roomId)
        {
            return _rooms.Remove(roomId);
        }

        public Room Find(int roomId)
        {
            Room room;
            _rooms.TryGetValue(roomId, out room);

            return room;
        }
    }
}
