using CSharpServer.Game;
using CSharpServer.Data;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

namespace CSharpServer
{
    public partial class ClientSession : PacketSession
    {
        public PlayerServerState ServerState { get; private set; } = PlayerServerState.ServerStateLogin;
        public Player CurrentPlayer { get; set; }
        public int SessionId { get; set; }
        List<ArraySegment<byte>> _reserveSendJobQueue = new List<ArraySegment<byte>>();
        long _heartBeatTick = 0;

#if GatherPacketForSend
        // 패킷 모아 보내기
        static readonly long minimumRequiredSize = 10000; // 최소 채워야 하는 바이트 수
        static readonly int sendTickInterval = 100;
        long _reservedSendBytes = 0;
        long _lastSendTick = 0;
#endif


        object _lock = new object();

#region Network
        public void HeartBeat()
        {
            if (_heartBeatTick > 0)
            {
                long delta = System.Environment.TickCount64 - _heartBeatTick;
                if (delta > 30 * 1000)
                {
                    Console.WriteLine($"Disconnected by HeartBeat Check");
                    Disconnect();
                    return;
                }
            }

            S_HeartBeat heartBeatPacket = new S_HeartBeat();
            Send(heartBeatPacket);

            RoomManager.Instance.PushFutureJob(5000, HeartBeat);
        }
        public void HandleHeartBeat()
        {
            _heartBeatTick = System.Environment.TickCount64;
        }

        // 메세지 예약만 하고 보내지는 않는다. 후에 다른 스레드에서 해당 작업을 꺼내서 Send 를 따로 처리한다.
        public void Send(IMessage packet)
        {
            // TODO: 이거 이렇게 안하고 패킷 일반화 시켜서 보낼 방법 없을까.. ProtoBuffer 에서 Enum 이름과 패킷(메세지) 이름이 같아지는 것을 막는다..
            // 후에 FlatBuffer 랑 FlexBuffer 도 써보면서 고민해보자. Equals, ToString 등 ProtoBuffer 처럼 잘 되어 있나 확인 필요.
            string msgName = packet.Descriptor.Name.Replace("_", string.Empty);
            MsgId msgId = (MsgId)Enum.Parse(typeof(MsgId), msgName);

            ushort size = (ushort)packet.CalculateSize();
            byte[] sendBuffer = new byte[size + 4];
            Array.Copy(BitConverter.GetBytes((ushort)(size + 4)), 0, sendBuffer, 0, sizeof(ushort));
            Array.Copy(BitConverter.GetBytes((ushort)msgId), 0, sendBuffer, 2, sizeof(ushort));
            Array.Copy(packet.ToByteArray(), 0, sendBuffer, 4, size);

#if DEBUG
            Console.WriteLine($"[Send] {packet.Descriptor.Name} {packet}");
#endif
            lock (_lock)
            {
                _reserveSendJobQueue.Add(sendBuffer);
#if GatherPacketForSend
                _reservedSendBytes += sendBuffer.Length;
#endif
            }
            //Send(new ArraySegment<byte>(sendBuffer));
        }

        public void FlushSend()
        {
            List<ArraySegment<byte>> sendJobList = null;
            lock (_lock)
            {
#if GatherPacketForSend
                long delta = System.Environment.TickCount64 - _lastSendTick;
                if (delta < sendTickInterval && _reservedSendBytes < minimumRequiredSize)
                {
                    return;
                }

                if (_reservedSendBytes > minimumRequiredSize)
                {
                    Console.WriteLine("해당 바이트 넘어서 패킷 보내는 중");
                }

                // 패킷 모아보낸 후 관련 사항 초기화
                _reservedSendBytes = 0;
                _lastSendTick = System.Environment.TickCount64;
#endif

                sendJobList = _reserveSendJobQueue;
                _reserveSendJobQueue = new List<ArraySegment<byte>>();
            }
            Send(sendJobList);
        }

        public override void OnConnected(EndPoint endPoint)
        {
            //Console.WriteLine($"OnConnected : {endPoint}");

            S_Connected connectedPacket = new S_Connected();
            Send(connectedPacket);

            // HeartBeat 세팅
            RoomManager.Instance.Push(HeartBeat);
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            PacketManager.Instance.OnRecvPacket(this, buffer);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            // TODO: 이 부분도 DB 작업이 다 처리가 안되었으면 Session 잡아두고 재로그인 불가능하게 하는 조치를 추가해야한다.
            SessionManager.Instance.Remove(this);

            RoomManager.Instance.Push(() =>
            {
                // TODO: room 선택하도록 바꿔야 함
                Room room = RoomManager.Instance.Find(1);
                if (room != null)
                {
                    if(CurrentPlayer == null)
                    {
                        return;
                    }

                    room.Push(room.LeaveGame, CurrentPlayer.Info.ActorId);
                }
            });
        }

        public override void OnSend(int numOfBytes)
        {
            //Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }
#endregion
    }
}
