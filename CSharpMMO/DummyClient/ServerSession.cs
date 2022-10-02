using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient
{
    class ServerSession : Session
    {
        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected : {endPoint}");

            S_PlayerInfoReq packet = new S_PlayerInfoReq() { playerId = 12, name = "David" };

            S_PlayerInfoReq.SkillInfo skillInfo = new S_PlayerInfoReq.SkillInfo();
            skillInfo.duration = 30;
            skillInfo.level = 1;
            skillInfo.id = 123;

            packet.skillInfos.Add(skillInfo);

            // 패킷 보내기
            //for (int i = 0; i < 5; i++)
            {
                var sendBuff = packet.Write();
                if(sendBuff != null)
                {
                    Send(sendBuff);
                }
            }
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnDisconnected : {endPoint}");
        }

        public override int OnRecv(ArraySegment<byte> buffer)
        {
            string recvStrData = Encoding.UTF8.GetString(buffer.Array, buffer.Offset, buffer.Count);
            Console.WriteLine("[Received] " + recvStrData);

            return buffer.Count;
        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"Transferred bytes: {numOfBytes}");
        }
    }
}
