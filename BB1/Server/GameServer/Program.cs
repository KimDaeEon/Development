using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using GameServer.Data;
using GameServer.DB;
using Google.Protobuf;
using ServerCore;
using System.Linq;
using System.Threading.Tasks;
using System.Timers;
using GameServer;

namespace Server
{
    // 1. Recv (N개)     서빙
    // 2. GameLogic (1)  요리사
    // 3. Send (1개)     서빙
    class Program
    {
        static Listener _listener = new Listener();
        static Connector _connector = new Connector();

        static void GameLogicTask()
        {
            while (true)
            {
                GameRoomManager.Instance.Update();
                Thread.Sleep(0);
            }
        }

        static void GameDbTask()
        {
            while (true)
            {
                DbJobProcessor.Instance.Flush();
                Thread.Sleep(100);
            }
        }

        static void Main(string[] args)
        {
            ConfigManager.Init();
            DataManager.Init();

            IPAddress ipAddr = IPAddress.Parse(ConfigManager.Config.ip);
            IPEndPoint endPoint = new IPEndPoint(ipAddr, ConfigManager.Config.port);
            _listener.Init(endPoint, () => { return SessionManager.Instance.Generate(); });

            Console.WriteLine("Listening on " + endPoint);

            // GameDbTask
            {
                Thread t = new Thread(GameDbTask);
                t.Name = "GameDB";
                t.Start();
            }


            GameRoomManager.Instance.AddRoom(dataSheetId: 1, zoneCells: 10, callback: (room) =>
            {
                if (room != null)
                {
                    room.Init(1, 10);
                    Console.WriteLine($"Room added successfully.");
                }
                else
                {
                    Console.WriteLine($"Adding room faield!!");
                }
            });

            // GameLogic
            Thread.CurrentThread.Name = "GameLogic";
            GameLogicTask();
        }
    }
}
