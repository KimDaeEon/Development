using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public class SkillComponent : EntityComponent
    {
        public override void Update()
        {
            Console.WriteLine("[SkillComponent] Updating skills...");

            int updatedValue = 100;
            Console.WriteLine($"[SkillComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Console.WriteLine("[SkillComponent] Notifying observers with message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Console.WriteLine($"[SkillComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Console.WriteLine($"[SkillComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Console.WriteLine($"[SkillComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Console.WriteLine("[SkillComponent] Unknown message type received.");
                    break;
            }
        }
    }
}