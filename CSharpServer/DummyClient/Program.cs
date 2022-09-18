﻿using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

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

            while (true)
            {
                try
                {
                    // Listen 소켓 생성
                    Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                    // Connect 요청
                    socket.Connect(endPoint);
                    Console.WriteLine($"Connected to {socket.RemoteEndPoint.ToString()}");

                    // 패킷 보내기
                    for (int i =0; i< 5; i++)
                    {
                        byte[] sendBuff = Encoding.UTF8.GetBytes($"Client Packet {i}");
                        int sendBytesCnt = socket.Send(sendBuff);
                    }

                    byte[] recvBuff = new byte[1024];
                    int recvBytesCnt = socket.Receive(recvBuff);
                    string recvStrData = Encoding.UTF8.GetString(recvBuff, 0, recvBytesCnt);

                    Console.WriteLine($"[Received] {recvStrData}");

                    socket.Shutdown(SocketShutdown.Both);
                    socket.Close();
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