using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class GameLogic : JobQueue
    {
        public static GameLogic Instance { get; } = new GameLogic();

        public void Update()
        {
            Flush();
        }
    }
}
