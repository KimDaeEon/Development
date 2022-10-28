using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class Projectile : Actor
    {
        public Data.SkillData Data { get; set; }
        public Projectile()
        {
            Type = ActorType.Projectile;
        }

        public override void Update()
        {

        }
    }
}
