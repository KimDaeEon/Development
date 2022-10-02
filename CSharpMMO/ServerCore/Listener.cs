using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public class Listener
    {
        Socket _listenSocket;
        Func<Session> _sessionFactory;

        public void Init(IPEndPoint endPoint, Func<Session> sessionFactory)
        {
            _listenSocket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            _sessionFactory += sessionFactory;

            // Bind
            _listenSocket.Bind(endPoint);

            // Listen 시작
            // 접속 대기열 숫자를 10으로 설정, backlog 라고 쓰여있는데 '밀린 일'이라는 뜻이다.
            _listenSocket.Listen(10);

            SocketAsyncEventArgs args = new SocketAsyncEventArgs();

            // 이렇게 해두면, 아래에서 AcceptAsync 가 true 가 떨어졌을 때에, 자동으로 OnAcceptCompleted 가 호출된다.
            // OnAcceptCompleted 는 새로운 스레드에서 실행이 된다.
            args.Completed += new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);
            RegisterAccept(args);
        }

        void RegisterAccept(SocketAsyncEventArgs args)
        {
            // 아래 부분을 안해두면 args 를 현재 재사용하고 있기에
            // args.AcceptSocket 에 남은 정보로 인해 두 번째 RegisterAccept 할 때에 크래시가 난다.
            args.AcceptSocket = null;

            bool pending = _listenSocket.AcceptAsync(args);

            // 현재 처리 중인 작업이 없어서 바로 Accept 성공이 된 경우
            if(pending == false)
            {
                OnAcceptCompleted(null, args);
            }

        }

        // 아래 함수는 멀티 스레드에서 여러 개의 함수가 접근가능한 부분이다.
        // 현재는 스레드별로 분리된 args 만 건드리기에 문제가 없다.
        // 하지만, 힙 영역에 있는 메모리를 여기서 건드리게 되면 이를 염두한 조치를 취해야 한다.
        void OnAcceptCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success)
            {
                Session session = _sessionFactory.Invoke();
                session.Start(args.AcceptSocket);
                session.OnConnected(args.AcceptSocket.RemoteEndPoint);
            }
            else
            {
                Console.WriteLine(args.SocketError.ToString());
            }

            // OnAcceptCompleted 작업이 끝났으면 다시 다음 연결 작업에 대한 처리를 해준다.
            RegisterAccept(args);
        }

        public Socket Accept()
        {
            _listenSocket.AcceptAsync();
            return _listenSocket.Accept();
        }
    }
}
