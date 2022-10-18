using CSharpServer.Contents;
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
    public class ClientSession : PacketSession
    {
        public Player CurrentPlayer { get; set; }
        public int SessionId { get; set; }

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
            Send(new ArraySegment<byte>(sendBuffer));
        }

        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected : {endPoint}");

            CurrentPlayer = ActorManager.Instance.Add<Player>();
            CurrentPlayer.Info.Name = $"Player_{CurrentPlayer.Info.ActorId}";
            CurrentPlayer.Info.PosInfo.MoveDir = MoveDir.Down;
            CurrentPlayer.Info.PosInfo.PosX = 0;
            CurrentPlayer.Info.PosInfo.PosX = 0;
            CurrentPlayer.Info.PosInfo.State = ActorState.Idle;
            CurrentPlayer.Session = this;

            StatInfo stat = null;
            // 임시로 1레벨 스탯 가져오도록, TODO: DB 에서 해당 정보 가져오도록 변경해야함.
            DataManager.StatDict.TryGetValue(1, out stat);
            CurrentPlayer.Stat.MergeFrom(stat); // Protobuf 는 MergeFrom 으로 값 복사 가능

            // TODO: 이 부분 나중에 Room 선택하는 방식으로
            Room room = RoomManager.Instance.Find(1);
            if(room != null)
            {
                room.Push(room.EnterGame, CurrentPlayer);
            }
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            PacketManager.Instance.OnRecvPacket(this, buffer);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            // TODO: room 선택하도록 바꿔야 함
            Room room = RoomManager.Instance.Find(1);
            if(room != null)
            {
                room.Push(room.LeaveGame, CurrentPlayer.Info.ActorId);
            } 

            SessionManager.Instance.Remove(this);
            Console.WriteLine($"OnDisconnected : {endPoint}");
        }

        public override void OnSend(int numOfBytes)
        {
            //Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }
    }
}
