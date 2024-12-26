using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public class QuestComponent : EntityComponent
    {
        public override void Update()
        {
            Console.WriteLine("[QuestComponent] Updating quest progress...");

            int updatedValue = 100;
            Console.WriteLine($"[QuestComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Console.WriteLine("[QuestComponent] Notifying observers with quest message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Console.WriteLine($"[QuestComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Console.WriteLine($"[QuestComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Console.WriteLine($"[QuestComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Console.WriteLine("[QuestComponent] Unknown message type received.");
                    break;
            }
        }
    }
}