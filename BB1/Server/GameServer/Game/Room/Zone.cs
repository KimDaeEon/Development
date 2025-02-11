using GameServer.DB.EFCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // Character 위치 이동 시에 새로운 Zone에 추가, 기존 Zone에서 제거
    // 사용자 위치를 기준으로 근처에 있는 Zone을 계산해서 패킷을 나눠 전달하는 용도로 사용
    public class Zone
    {
        public int IndexX { get; private set; } 
        public int IndexY { get; private set; }

        public HashSet<Character> Characters { get; set; } = new HashSet<Character>();
        public HashSet<Monster> Monsters { get; set; } = new HashSet<Monster>();

        public Zone(int x, int y)
        {
            IndexX = x;
            IndexY = y;
        }

        public void Add(Entity entity)
        {
            if (entity is Character character)
            {
                Characters.Add(character);
            }
            else if (entity is Monster monster)
            {
                Monsters.Add(monster);
            }
            else
            {
                Console.WriteLine($"[Zone] Entity of type {entity.GetType().Name} cannot be added.");
            }
        }

        public void Remove(Entity entity)
        {
            if (entity is Character character)
            {
                Characters.Remove(character);
            }
            else if (entity is Monster monster)
            {
                Monsters.Remove(monster);
            }
            else
            {
                Console.WriteLine($"[Zone] Entity of type {entity.GetType().Name} cannot be removed.");
            }
        }


        public void BroadCast<T>(T packet, HashSet<Character> excludeCharacters = null) where T : Google.Protobuf.IMessage
        {
            foreach (var character in Characters)
            {
                // 제외할 Character 목록에 포함되어 있으면 패킷을 보내지 않음
                if (excludeCharacters != null && excludeCharacters.Contains(character))
                {
                    continue;
                }

                character.Session?.Send(packet);
            }
        }

    }
}
