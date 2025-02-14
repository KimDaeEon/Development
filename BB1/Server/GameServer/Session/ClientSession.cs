using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

namespace GameServer
{
    public partial class ClientSession : PacketSession
    {
        public long AccountDbId { get; set; }
        public int SessionId { get; set; }

        // https://stackoverflow.com/questions/5209623/is-a-reference-assignment-threadsafe
        public bool IsVerified { get; set; } // IO 스레드에서만 건드리도록 하자. bool Type은 assign thread-safe 하기에 읽기만 할 시에는 lock 제외

        object _lock = new object();

        #region Network
        // 예약만 하고 보내지는 않는다
        public void Send(IMessage packet)
        {
            Send(new ArraySegment<byte>(MakeSendBuffer(packet)));
        }

        public static byte[] MakeSendBuffer(IMessage packet)
        {
            MsgId msgId = (MsgId)Enum.Parse(typeof(MsgId), packet.Descriptor.Name);
            ushort size = (ushort)packet.CalculateSize();
            byte[] sendBuffer = new byte[size + 4];
            Array.Copy(BitConverter.GetBytes((ushort)(size + 4)), 0, sendBuffer, 0, sizeof(ushort));
            Array.Copy(BitConverter.GetBytes((ushort)msgId), 0, sendBuffer, 2, sizeof(ushort));
            Array.Copy(packet.ToByteArray(), 0, sendBuffer, 4, size);
            return sendBuffer;
        }

        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected : {endPoint}");
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            PacketManager.Instance.OnRecvPacket(this, buffer);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            SessionManager.Instance.Remove(this);

            Console.WriteLine($"OnDisconnected : {endPoint}");


            // TODO: MyCharacter랑 Room이 게임 스레드에서만 할당 및 해제가 이루어져서 읽기만은 문제 없을 것이라 판단해서 이렇게 했는데, 혹시나 문제 생기면 이 부분도 확인
            var curRoom = MyCharacter?.Room;
            if (curRoom != null)
            {
                GameRoomManager.Instance.FindRoom(curRoom.GameRoomId, callback: (room) =>
                {
                    if (room != null)
                    {
                        Console.WriteLine($"Found Room ID: {room.GetHashCode()}");

                        room.HandleLeaveGame(this);
                    }
                });
            }
        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }
        #endregion
    }
}
