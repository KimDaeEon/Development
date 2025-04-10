using Google.Protobuf.Protocol;
using Microsoft.EntityFrameworkCore.Update.Internal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class VisionCellsComponent : EntityComponent
    {
        public Character OwnerCharacter
        {
            get
            {
                return (this.Owner) as Character;
            }
        }

        public HashSet<Entity> PreviousAdjacentEntities { get; private set; } = new HashSet<Entity>();

        
        public override void Update()
        {
            if (Owner == null || Owner.Room == null)
            {
                return;
            }

            Console.WriteLine("[VisionCellsComponent] Updating...");

            HashSet<Entity> excludedEntities = new HashSet<Entity> { Owner };

            HashSet<Entity> currentAdjacentEntities = Owner.Room.GetAdjacentEntities(OwnerCharacter.GetCellPosFromPosition(), excludedEntities);

            List<Entity> addedEntities = currentAdjacentEntities.Except(PreviousAdjacentEntities).ToList();

            if (addedEntities.Count > 0)
            {
                S_Spawn spawnPacket = new S_Spawn();

                foreach (Entity entity in addedEntities)
                {
                    spawnPacket.EntityInfos.Add(entity.EntityInfo);
                }

                OwnerCharacter.Session?.Send(spawnPacket);
            }

            // 기존엔 있었는데 사라진 애들 Despawn 처리
            List<Entity> removedEntities = PreviousAdjacentEntities.Except(currentAdjacentEntities).ToList();
            if (removedEntities.Count > 0)
            {
                S_Despawn despawnPacket = new S_Despawn();

                foreach (Entity entity in removedEntities)
                {
                    despawnPacket.GameIds.Add(entity.GameId);
                }

                OwnerCharacter.Session?.Send(despawnPacket);
            }

            // 교체
            PreviousAdjacentEntities = currentAdjacentEntities;

            Owner.Room.PushFutureJob(100, Update);
        }

        public override void Notify(EventData message)
        {
            Console.WriteLine("[VisionCellsComponent] Notifying observers with message.");
            NotifyObservers(message);
        }

        public override void OnNotify(EventData message)
        {
            switch (message.Type)
            {
                case EventDataType.Int:
                    Console.WriteLine($"[VisionCellsComponent] Received int message: {(int)message.Data}");
                    break;
                case EventDataType.Float:
                    Console.WriteLine($"[VisionCellsComponent] Received float message: {(float)message.Data}");
                    break;
                case EventDataType.String:
                    Console.WriteLine($"[VisionCellsComponent] Received string message: {(string)message.Data}");
                    break;
                default:
                    Console.WriteLine("[VisionCellsComponent] Unknown message type received.");
                    break;
            }
        }
    }
}
