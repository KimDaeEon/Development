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
    public class StatusComponent : EntityComponent
    {
        public override void Update()
        {
            Logger.LogDebug("[StatusComponent] Updating status...");

            int updatedValue = 100;
            Logger.LogDebug($"[StatusComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Logger.LogDebug("[StatusComponent] Notifying observers with message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Logger.LogDebug($"[StatusComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Logger.LogDebug($"[StatusComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Logger.LogDebug($"[StatusComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Logger.LogDebug("[StatusComponent] Unknown message type received.");
                    break;
            }
        }
    }
}
