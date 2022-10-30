using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    // 클라이언트와 서버 연결은 물론 서버 끼리의 통신을 위한 범용적인 클래스
    public class Connector
    {
        Func<Session> _sessionFactory;

        public void Connect(IPEndPoint endPoint, Func<Session> sessionFactory)
        {
            Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            _sessionFactory = sessionFactory;

            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.Completed += OnConnectCompleted;
            args.RemoteEndPoint = endPoint;
            args.UserToken = socket; // 소켓을 args 내부에 저장, 연결 별로 독립적인 socket 사용을 위한 것

            // 더미 클라이언트 테스트 시에 동시에 1000개의 접속 요청을 보내면 접속이 누락되는데 그러한 상황을 막기 위한 것
            Thread.Sleep(3);
            RegisterConnect(args);
        }

        void RegisterConnect(SocketAsyncEventArgs args)
        {
            Socket socket = args.UserToken as Socket;

            if (socket == null)
            {
                return;
            }

            bool pending = socket.ConnectAsync(args);
            if (pending == false)
            {
                OnConnectCompleted(null, args);
            }
        }

        void OnConnectCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success)
            {
                Session session = _sessionFactory.Invoke();
                session.Start(args.ConnectSocket);

                // 아래 결과는 같다고 나온다. socket.ConnectAsync 를 하는 순간 내부적으로 args 의 ConnectSocket에 socket 의 주소를 저장하는 것 같다.
                //Console.WriteLine("args.ConnectSocket 과 args.UserToken.ConnectSocket 이 같은가? " + (args.ConnectSocket == args.UserToken));
                session.OnConnected(args.ConnectSocket.RemoteEndPoint);
            }
            else
            {
                //Console.WriteLine($"Connect Fail! reason:{args.SocketError}");
            }
        }
    }
}
