using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Contents
{
    public class Projectile : Actor
    {
        public Data.Skill Data { get; set; }
        public Projectile()
        {
            Type = ActorType.Projectile;
        }

        public override void Update()
        {

        }
    }
}
