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
        class Packet
        {
            public ushort size;
            public ushort id;
        }
        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected : {endPoint}");

            ArraySegment<byte> openSegment = SendBufferHelper.Open(4096);
            byte[] buffer = Encoding.UTF8.GetBytes("Hello Client");
            Array.Copy(buffer, 0, openSegment.Array, openSegment.Offset, buffer.Length);
            ArraySegment<byte> sendBuff = SendBufferHelper.Close(buffer.Length);

            // TODO: 아래 send 부분 멀티 스레드에서 문제 없나 한 번 더 생각해보기.
            // ThreadLocal로 해결이 된 거라고 생각이 되는데, 그게 없어도 된다는 식으로 설명해주셔서..
            //Send(sendBuff);

            Thread.Sleep(50000);
            Disconnect();
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            PacketManager.Instance.OnRecvPacket(this, buffer);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnDisconnected : {endPoint}");
        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }
    }
}
