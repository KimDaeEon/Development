using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Contents
{
    public class Player : Actor
    {
        public ClientSession Session { get; set; }

        public Player()
        {
            Type = ActorType.Player;
        }

        public static bool IsInValidSessionAndPlayerAndRoom(ClientSession session, ref Player player, ref Room room)
        {
            if (null == session)
            {
                return true;
            }

            player = session.CurrentPlayer;
            if (null == player)
            {
                return true;
            }

            room = player.CurrentRoom;
            if (null == room)
            {
                return true;
            }

            return false;
        }

        public override void OnDamaged(Actor attacker, int damage)
        {
            base.OnDamaged(attacker, damage);
        }

        public override void OnDead(Actor attacker)
        {
            base.OnDead(attacker);
        }
    }
}
