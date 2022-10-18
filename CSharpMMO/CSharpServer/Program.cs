using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using CSharpServer.Contents;
using CSharpServer.Data;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;

namespace CSharpServer
{

    class Program
    {
        static Listener _listener = new Listener();
        static List<System.Timers.Timer> _timers = new List<System.Timers.Timer>();
        static void TickRoom(Room room, int tick = 100)
        {
            var timer = new System.Timers.Timer();
            timer.Interval = tick;
            timer.Elapsed += ((sender, elapsedEventArgs) => { room.Push(room.Update); });
            timer.AutoReset = true;
            timer.Enabled = true; // 이것을 하는 순간 timer 가 실행이 된다.

            _timers.Add(timer);
        }

        static void Main(string[] args)
        {
            // 데이터 로딩
            ConfigManager.Init();
            DataManager.Init();

            var d = DataManager.StatDict;
            
            // 서버 시작 후 초기화해줄 부분들
            Room room = RoomManager.Instance.Add(1);
            TickRoom(room, 50);

            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정

            try
            {
                _listener.Init(endPoint, () => { return SessionManager.Instance.Generate(); });
                Console.WriteLine("Listening...");

                while (true)
                {
                    Thread.Sleep(100);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}
