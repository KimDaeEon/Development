using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    class Session
    {
        Socket _socket;
        int _disconnected = 0;

        public void Start(Socket socket)
        {
            _socket = socket;

            SocketAsyncEventArgs recvArgs = new SocketAsyncEventArgs();
            recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvCompleted);

            // Buffer 시작점이 2번째 인자로 들어가는데, 엄청나게 큰 버퍼를 만들어놓고,
            // 그것을 Session 마다 쪼개 쓰는 경우도 있어서 시작점이 2번째 인자로 들어간다.
            recvArgs.SetBuffer(new byte[1024], 0, 1024);

            RegisterRecv(recvArgs);
        }

        public void Send(byte[] sendBuff)
        {
            _socket.Send(sendBuff);
        }

        public void Disconnect()
        {
            // 이렇게 해야 Disconnect 2번 해도 문제가 생기지 않는다.
            // 그런데 예제와는 다르게 현재 Disconnect 를 2번 호출해도 에러가 안나온다.
            // 네트워크 관련 라이브러리가 내부적으로 변경이 되어 이렇게 된 것 같다.
            // 일단 Disconnect() 를 멀티 스레드 환경에서 2번 호출해도 문제가 없게 하려면 이런 식으로 하면 된다는 것만 이해하자.
            if(Interlocked.Exchange(ref _disconnected, 1) == 1)
            {
                return;
            }
            _socket.Shutdown(SocketShutdown.Both);
            _socket.Close();
        }

        #region 통신관련 내부 메서드
        void RegisterRecv(SocketAsyncEventArgs args)
        {
            bool pending = _socket.ReceiveAsync(args);
            if (pending == false)
            {
                OnRecvCompleted(null, args);
            }

        }

        void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
            {
                try
                {
                    Console.WriteLine("offset is " + args.Offset);
                    string recvStrData = Encoding.UTF8.GetString(args.Buffer, args.Offset, args.BytesTransferred);
                    Console.WriteLine("[Received] " + recvStrData);
                    RegisterRecv(args);

                }
                catch (Exception e)
                {
                    Console.WriteLine($"OnRecvCompleted Failed {e}");
                }
            }
        }
        #endregion
    }
}
