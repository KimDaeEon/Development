using System;
using System.Net;
using System.Threading;
using ServerCore;

namespace DummyClient
{


    class Program
    {
        static void Main(string[] args)
        {
            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정
            Connector connector = new Connector();

            int sessionCount = 1;
            connector.Connect(endPoint, () => { return SessionManager.Instance.Generate(); });

            while (true)
            {
                try
                {
                    SessionManager.Instance.SendForEach();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }

                // 이동 패킷을 보통 1초에 4번 보낸다고 한다.
                Thread.Sleep(100);
            }
        }
    }
}
