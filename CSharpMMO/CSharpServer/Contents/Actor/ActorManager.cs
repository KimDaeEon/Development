using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Contents
{
    public class ActorManager
    {
        public static ActorManager Instance { get; } = new ActorManager();

        object _lock = new object();
        Dictionary<int, Player> _players = new Dictionary<int, Player>();

        // [Unused(1)][ActorType(7)][ActorId(24)], 1비트는 부호 비트라서 사용하지 않음.
        int _lastIssuedId = 1;  // Actor 에게 부여되는 Id

        public T Add<T>() where T : Actor, new()
        {
            T actor = new T();

            lock (_lock)
            {
                actor.Id = GenerateId(actor.Type);

                if (actor.Type == ActorType.Player)
                {
                    _players.Add(actor.Id, actor as Player);
                }
            }

            return actor;
        }

        int GenerateId(ActorType type)
        {
            lock (_lock)
            {
                // 맨 앞 8비트 쪽에 type 넣고, 그 아래 24비트에 생성된 순서를 Id로 부여
                return ((int)type << 24) | (_lastIssuedId++);
            }
        }

        public static ActorType GetActorTypeById(int id)
        {
            return (ActorType)(id >> 24 & 0x7F); // 혹시 몰라서 비트 연산
        }

        public bool Remove(int actorId)
        {
            ActorType actorType = GetActorTypeById(actorId);
            lock (_lock)
            {
                if (actorType == ActorType.Player)
                {
                    return _players.Remove(actorId);
                }
            }

            return false;
        }

        public Player Find(int actorId)
        {
            ActorType actorType = GetActorTypeById(actorId);

            lock (_lock)
            {
                if (actorType == ActorType.Player)
                {
                    Player player;
                    _players.TryGetValue(actorId, out player);
                    return player;
                }
            }

            return null;
        }
    }
}
