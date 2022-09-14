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
        static void Main(string[] args)
        {
            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정

            Console.WriteLine(ipHost.AddressList);
            Console.WriteLine(ipAddr);

            try
            {
                // Listen 소켓 생성
                Socket listenSocket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                // Bind
                listenSocket.Bind(endPoint);

                // Listen 시작
                // 접속 대기열 숫자를 10으로 설정, backlog 라고 쓰여있는데 '밀린 일'이라는 뜻이다.
                listenSocket.Listen(10);

                while (true)
                {
                    Console.WriteLine("Listening...");

                    Socket clientSocket = listenSocket.Accept(); // 접속 요청을 기다리고, 접속 요청이 오면 client 용 socket 을 생성

                    byte[] recvBuff = new byte[1024];
                    int recvBytesCnt = clientSocket.Receive(recvBuff);
                    string recvStrData = Encoding.UTF8.GetString(recvBuff, 0, recvBytesCnt);
                    Console.WriteLine(recvStrData);

                    byte[] sendBuff = Encoding.UTF8.GetBytes("Hello, Client");
                    clientSocket.Send(sendBuff);

                    // Client 와 연결 종료
                    clientSocket.Shutdown(SocketShutdown.Both);
                    clientSocket.Close();
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}
