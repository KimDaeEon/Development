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
            connector.Connect(endPoint, () => { return new ServerSession(); });
             
            while (true)
            {
                try
                {
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }

                Thread.Sleep(100);
            }
        }
    }
}
