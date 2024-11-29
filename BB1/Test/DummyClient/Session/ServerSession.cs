using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient
{
    public class ServerSession : PacketSession
    {
        public int DummyId { get; set; }
        public override void OnConnected(EndPoint endPoint)
        {
            //Console.WriteLine($"OnConnected : {endPoint}");

            //// CustomHandler 등록, 이를 통해 메인 스레드에서 패킷이 처리되어 게임 오브젝트 관련 작업을 처리한다.
            //PacketManager.Instance.CustomHandler = (session, packet, packetId) =>
            //{
            //    PacketQueue.Instance.Push(packetId, packet);
            //};
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            //Console.WriteLine($"OnDisconnected : {endPoint}");
            //Console.WriteLine($"OnDisconnected : {endPoint}");
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            PacketManager.Instance.OnRecvPacket(this, buffer);
        }

        public override void OnSend(int numOfBytes)
        {
            //Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }

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

            Send(new ArraySegment<byte>(sendBuffer));
        }
    }
}
