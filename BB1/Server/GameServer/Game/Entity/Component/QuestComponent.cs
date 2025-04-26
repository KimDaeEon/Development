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
    public class QuestComponent : EntityComponent
    {
        public override void Update()
        {
            Logger.LogDebug("[QuestComponent] Updating quest progress...");

            int updatedValue = 100;
            Logger.LogDebug($"[QuestComponent] New value: {updatedValue}");

            NotifyObservers(new EventData { Type = EventDataType.Int, Data = updatedValue });
        }

        public override void Notify(EventData message)
        {
            Logger.LogDebug("[QuestComponent] Notifying observers with quest message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Logger.LogDebug($"[QuestComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Logger.LogDebug($"[QuestComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Logger.LogDebug($"[QuestComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Logger.LogDebug("[QuestComponent] Unknown message type received.");
                    break;
            }
        }
    }
}