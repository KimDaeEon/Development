using System;
using System.Net;
using System.Threading;
using ServerCore;

namespace CSharpServer
{

    class Program
    {
        static Listener _listener = new Listener();
        public static GameRoom _room = new GameRoom();
        static void FlushRoom()
        {
            _room.Push(() => _room.Flush());
            JobTimer.Instance.Push(FlushRoom, 250);
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
                _listener.Init(endPoint, () => { return SessionManager.Instance.Generate(); });
                Console.WriteLine("Listening...");

                //FlushRoom(); // 아래처럼 하는 것이 더 직관적인 것 같다.
                JobTimer.Instance.Push(FlushRoom, 250);

                while (true)
                {
                    JobTimer.Instance.Flush();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}
