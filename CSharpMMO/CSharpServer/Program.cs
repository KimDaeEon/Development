using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using CSharpServer.Game;
using CSharpServer.Data;
using CSharpServer.DB;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System.Linq;
using CSharpServer.Utils;
using System.Threading.Tasks;

namespace CSharpServer
{
    // 스레드 구조
    // 네트워크 패킷 Recv 처리 스레드 (N개)
    // GameLogic 처리 스레드 (1개)
    // DB 처리 스레드 (1개)
    // 네트워크 패킷 Send 처리 스레드 (1개)
    class Program
    {
        static Listener _listener = new Listener();
        static void GameLogicTask()
        {
            while (true)
            {
                RoomManager.Instance.Update();
                Thread.Sleep(0);
            }
        }

        static void DbTask()
        {
            while (true)
            {
                // TODO: 아래 부분도 분산을 하는 방식으로
                DbJobQueue.Instance.Flush();
                Thread.Sleep(0);
            }
        }

        static void NetworkSendTask()
        {
            while (true)
            {
                List<ClientSession> sessions = SessionManager.Instance.GetSessions();
                foreach(ClientSession session in sessions)
                {
                    session.FlushSend();
                }

                Thread.Sleep(0);
            }
        }

        static void Main(string[] args)
        {
            // 데이터 로딩
            ConfigManager.Init();
            DataManager.Init();

            // 게임 룸 세팅
            RoomManager.Instance.Push(() =>
            {
                RoomManager.Instance.Add(1);
            });

            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정

            try
            {
                _listener.Init(endPoint, () => { return SessionManager.Instance.Generate(); });
                Console.WriteLine("Listening...");
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            // 네트워크 Send 스레드실행
            {
                Thread t = new Thread(NetworkSendTask);
                t.Name = "Network Send";
                t.Start();
            }

            // Db 작업 스레드 실행
            {
                Thread t = new Thread(DbTask);
                t.Name = "DB";
                t.Start();
            }

            // 게임 로직은 메인 스레드에서
            Thread.CurrentThread.Name = "GameLogic";
            GameLogicTask();
        }
    }
}
