using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class StatusComponent : EntityComponent
    {
        public override void Update()
        {
            Console.WriteLine("[StatusComponent] Updating status...");

            int updatedValue = 100;
            Console.WriteLine($"[StatusComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Console.WriteLine("[StatusComponent] Notifying observers with message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Console.WriteLine($"[StatusComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Console.WriteLine($"[StatusComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Console.WriteLine($"[StatusComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Console.WriteLine("[StatusComponent] Unknown message type received.");
                    break;
            }
        }
    }
}
