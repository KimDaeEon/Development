using System;
using ServerCore;

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
            Logger.LogDebug("[AIComponent] Updating AI logic...");

            int updatedValue = 100;
            Logger.LogDebug($"[AIComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Logger.LogDebug("[AIComponent] Notifying observers with message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Logger.LogDebug($"[AIComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Logger.LogDebug($"[AIComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Logger.LogDebug($"[AIComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Logger.LogDebug("[AIComponent] Unknown message type received.");
                    break;
            }
        }
    }
}