using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public class AIComponent : EntityComponent
    {
        public override void Update()
        {
            Console.WriteLine("[AIComponent] Updating AI logic...");

            int updatedValue = 100;
            Console.WriteLine($"[AIComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Console.WriteLine("[AIComponent] Notifying observers with message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Console.WriteLine($"[AIComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Console.WriteLine($"[AIComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Console.WriteLine($"[AIComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Console.WriteLine("[AIComponent] Unknown message type received.");
                    break;
            }
        }
    }
}