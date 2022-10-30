using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    // Actor 가 어떤 Cell 에 있을 때에 어떤 영역(Zone) 에 속하는지를 알아내기 위해 존재하는 클래스
    public class Zone
    {
        public int IndexY { get; private set; }
        public int IndexX { get; private set; }
        public HashSet<Player> Players { get; set; } = new HashSet<Player>();
        public HashSet<Monster> Monsters { get; set; } = new HashSet<Monster>();
        public HashSet<Projectile> Projectiles { get; set; } = new HashSet<Projectile>();
        public Zone(int y, int x)
        {
            // 자신이 몇 번째 idx 의 zone 인지 기록해둔다.
            IndexY = y;
            IndexX = x;
        }
        public void Remove(Actor actor)
        {
            ActorType type = ActorManager.GetActorTypeById(actor.Id);

            switch (type)
            {
                case ActorType.Player:
                    Players.Remove((Player)actor);
                    break;
                case ActorType.Monster:
                    Monsters.Remove((Monster)actor);
                    break;
                case ActorType.Projectile:
                    Projectiles.Remove((Projectile)actor);
                    break;
            }
        }
        public Player FindPlayer(Func<Player, bool> condition)
        {
            foreach(Player player in Players)
            {
                if (condition.Invoke(player))
                {
                    return player;
                }
            }

            return null;
        }

        public List<Player> FindAllPlayers(Func<Player, bool> condition)
        {
            List<Player> foundList = new List<Player>();

            foreach (Player player in Players)
            {
                if (condition.Invoke(player))
                {
                    foundList.Add(player);
                }
            }

            return foundList;
        }
    }
}
