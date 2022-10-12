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
    class ClientSession : PacketSession
    {
        public int SessionId { get; set; }
        public float PosX { get; set; }
        public float PosY { get; set; }
        public float PosZ { get; set; }

        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected : {endPoint}");

            // PROTO Test
            S_Chat chat = new S_Chat()
            {
                Context = "안녕하세요"
            };

            ushort size = (ushort)chat.CalculateSize();
            byte[] sendBuffer = new byte[size + 4];
            Array.Copy(BitConverter.GetBytes(size + 4), 0, sendBuffer, 0, sizeof(ushort));
            ushort protocolId = (ushort)MsgId.SChat;
            Array.Copy(BitConverter.GetBytes(protocolId), 0, sendBuffer, 2, sizeof(ushort));
            Array.Copy(chat.ToByteArray(), 0, sendBuffer, 4, size);

            Send(new ArraySegment<byte>(sendBuffer));
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            //PacketManager.Instance.OnRecvPacket(this, buffer);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            Console.WriteLine($"Thread:{Thread.CurrentThread.ManagedThreadId} OnDisconnected : {endPoint}");
            SessionManager.Instance.Remove(this);
        }

        public override void OnSend(int numOfBytes)
        {
            //Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }
    }
}
