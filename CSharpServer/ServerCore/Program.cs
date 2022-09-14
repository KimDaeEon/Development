using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    class Program
    {
        static Listener _listener = new Listener();
        static void OnAcceptHandler(Socket clientSocket)
        {
            try
            {
                Session session = new Session();
                session.Start(clientSocket);

                byte[] sendBuff = Encoding.UTF8.GetBytes("Hello Client");
                session.Send(sendBuff);

                Thread.Sleep(500);
                session.Disconnect();
            }
            catch(Exception e)
            {

            }
        }

        static void Main(string[] args)
        {
            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정

            try
            {

                _listener.Init(endPoint, OnAcceptHandler);
                Console.WriteLine("Listening...");
                while (true)
                {
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}
