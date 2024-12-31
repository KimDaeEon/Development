using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // TODO: 이게 어차피 한 개일 거라 JobQueue 를 달아둬야 할지.. Room도 웬만해서는 유지될 거고.. 네트워크 스레드랑 연결해서 추후 최적화 할 때에 좀 더 전체 스레드 구조 좀 더 고민해보기
    public partial class GameRoomManager : JobQueue
    {
        public static GameRoomManager Instance { get; } = new GameRoomManager();

        private readonly Dictionary<int, GameRoom> _rooms = new Dictionary<int, GameRoom>();
        private int _roomIdIssuer = 1;

        public void Update()
        {
            Flush();

            foreach (var room in _rooms.Values)
            {
                room.Update();
            }
        }

        public void AddRoom(int dataSheetId, int zoneCells, Action<GameRoom> callback = null)
        {
            PushJob(() =>
            {
                int roomId = _roomIdIssuer++;
                var newRoom = new GameRoom();
                newRoom.Init(dataSheetId, zoneCells);

                _rooms[roomId] = newRoom;

                callback?.Invoke(newRoom);
            });
        }

        public void FindRoom(int roomId, Action<GameRoom> callback)
        {
            PushJob(() =>
            {
                if (_rooms.TryGetValue(roomId, out var room))
                {
                    Console.WriteLine($"[GameRoomManager] Room {roomId} found.");
                    callback?.Invoke(room);
                    return;
                }

                Console.WriteLine($"[GameRoomManager] Room {roomId} not found.");
                callback?.Invoke(null);
            });
        }

        public void RemoveRoom(int roomId, Action<GameRoom> callback = null)
        {
            PushJob(() =>
            {
                if (_rooms.TryGetValue(roomId, out var room))
                {
                    _rooms.Remove(roomId);
                    Console.WriteLine($"[GameRoomManager] Room {roomId} removed.");
                    callback?.Invoke(room);
                    return;
                }

                Console.WriteLine($"[GameRoomManager] Room {roomId} removal failed. Room not found.");
                callback?.Invoke(null);
            });
        }

        public void GetAllRooms(Action<List<GameRoom>> callback)
        {
            PushJob(() =>
            {
                var rooms = _rooms.Values.ToList();
                callback?.Invoke(rooms);
            });
        }
    }
}
