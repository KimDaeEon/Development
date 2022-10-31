using System;
using System.Net;
using System.Threading;
using ServerCore;

namespace DummyClient
{
    class Program
    {
        static int dummyClientsCount = 1000;

        static void Main(string[] args)
        {
            Thread.Sleep(2000);

            while (true)
            {
                Console.Clear();
                Console.WriteLine("더미 클라이언트를 시작하시려면 s 키를 눌러주세요.");
                ConsoleKeyInfo ckey = Console.ReadKey();

                if(ckey.Key == ConsoleKey.S)
                {
                    break;
                }
            }

            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[1];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정
            Connector connector = new Connector();


            for (int i = 0; i < dummyClientsCount; i++)
            {
                connector.Connect(endPoint, () => { return SessionManager.Instance.Generate(); });
            }

            while (true)
            {
                try
                {
                    //SessionManager.Instance.SendForEach();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }

                // 이동 패킷을 보통 1초에 4번 보낸다고 한다.
                Thread.Sleep(1000);
            }
        }
    }
}
