using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CSharpServer
{
    class GameRoom : IJobQueue
    {
        List<ClientSession> _sessions = new List<ClientSession>();
        JobQueue _jobQueue = new JobQueue();
        List<ArraySegment<byte>> _pendingList = new List<ArraySegment<byte>>();

        public void Push(Action job)
        {
            _jobQueue.Push(job);
        }

        public void Enter(ClientSession session)
        {
            // 플레이어 추가
            _sessions.Add(session);
            session.Room = this;

            // 들어온 플레이어에게 기존에 있던 플레이어 목록 전송
            S_PlayerList playerListPkt = new S_PlayerList();
            foreach(ClientSession _session in _sessions)
            {
                playerListPkt.players.Add(new S_PlayerList.Player()
                {
                    isSelf = ( session == _session ),
                    playerId = _session.SessionId,
                    posX = _session.PosX,
                    posY = _session.PosY,
                    posZ = _session.PosZ
                });
            }
            session.Send(playerListPkt.Write());

            // 다른 모든 플레이어에게 해당 플레이어가 입장했음을 전송
            S_BroadcastEnterGame enterPkt = new S_BroadcastEnterGame();
            enterPkt.playerId = session.SessionId;
            
            // 처음 시작은 0,0,0 좌표에서
            enterPkt.posX = 0;
            enterPkt.posY = 0;
            enterPkt.posZ = 0;

            Broadcast(enterPkt.Write());
        }

        public void Leave(ClientSession session)
        {
            _sessions.Remove(session);

            // 다른 모든 플레이어에게 해당 플레이어가 나갔음을 전송
            S_BroadcastLeaveGame leavePkt = new S_BroadcastLeaveGame();
            leavePkt.playerId = session.SessionId;

            Broadcast(leavePkt.Write());
        }

        public void Flush()
        {
            foreach (ClientSession tempSession in _sessions)
            {
                tempSession.Send(_pendingList);
            }

            //Console.WriteLine($"Flushed {_pendingList.Count}");
            _pendingList.Clear();
        }

        public void Move(ClientSession session, C_Move packet)
        {
            // 좌표 이동
            session.PosX = packet.posX;
            session.PosY = packet.posY;
            session.PosZ = packet.posZ;

            // 모두에게 전송
            S_BroadcastMove broadcastMovePacket = new S_BroadcastMove();
            broadcastMovePacket.playerId = session.SessionId;
            broadcastMovePacket.posX = session.PosX;
            broadcastMovePacket.posY = session.PosY;
            broadcastMovePacket.posZ = session.PosZ;

            Broadcast(broadcastMovePacket.Write());
        }

        public void Broadcast(ArraySegment<byte> segment)
        {
            _pendingList.Add(segment);
            //foreach (ClientSession tempSession in _sessions)
            //{
            //    tempSession.Send(segment);
            //}
        }
    }
}
