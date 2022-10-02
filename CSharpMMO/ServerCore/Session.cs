using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    public abstract class PacketSession : Session
    {
        public static readonly int HeaderSize = 2;
        public sealed override int OnRecv(ArraySegment<byte> buffer)
        {
            int processedLen = 0;

            while (true)
            {
                // buffer 가 2바이트보다 작으면 헤더 부분을 파싱해서 이후 작업을 할 수 없으므로 작업 중지
                if (buffer.Count < HeaderSize)
                {
                    break;
                }

                // 이제 헤더를 확인 가능하므로 패킷이 다 도착했는지 확인
                ushort dataSize = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
                if (buffer.Count < dataSize)
                {
                    break;
                }

                // 받은 패킷에 대한 처리(조립)
                // 중요한 것은 ArraySegment 는 struct stack 에 할당된다. 그래서 new 도 상관없음.
                OnRecvPacket(new ArraySegment<byte>(buffer.Array, buffer.Offset, dataSize));

                // 버퍼에서 처리된 것만큼 Offset 뒤로 밀어주고, Count 를 줄여준다.
                processedLen += dataSize;
                buffer = new ArraySegment<byte>(buffer.Array, buffer.Offset + dataSize, buffer.Count - dataSize);
            }

            return processedLen;
        }

        public abstract void OnRecvPacket(ArraySegment<byte> buffer);
    }
    public abstract class Session
    {
        Socket _socket;
        int _disconnected = 0;

        RecvBuffer _recvBuffer = new RecvBuffer(1024);

        SocketAsyncEventArgs _recvArgs = new SocketAsyncEventArgs();
        SocketAsyncEventArgs _sendArgs = new SocketAsyncEventArgs(); // 보낼 때마다 생성하기 보다는 맴버로 갖도록
        List<ArraySegment<byte>> _pendingList = new List<ArraySegment<byte>>(); // 패킷 송신 대기 중인 데이터

        Queue<ArraySegment<byte>> _sendQueue = new Queue<ArraySegment<byte>>();

        object _lock = new object();

        public abstract void OnConnected(EndPoint endPoint);
        public abstract int OnRecv(ArraySegment<byte> buffer);
        public abstract void OnSend(int numOfBytes);
        public abstract void OnDisconnected(EndPoint endPoint);

        void Clear()
        {
            lock (_lock)
            {
                _sendQueue.Clear();
                _pendingList.Clear();
            }
        }
        public void Start(Socket socket)
        {
            _socket = socket;

            // 수신 콜백 설정
            _recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvCompleted);

            // Buffer 시작점이 2번째 인자로 들어가는데, 엄청나게 큰 버퍼를 만들어놓고,
            // 그것을 Session 마다 쪼개 쓰는 경우도 있어서 시작점이 2번째 인자로 들어간다.
            _recvArgs.SetBuffer(new byte[1024], 0, 1024);

            // 송신 콜백 설정
            _sendArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnSendCompleted);

            RegisterRecv();
        }

        public void Send(ArraySegment<byte> sendBuff)
        {
            lock (_lock)
            {
                _sendQueue.Enqueue(sendBuff);
                if (_pendingList.Count == 0)
                {
                    RegisterSend();
                }
                else
                {

                }
            }
        }

        public void Disconnect()
        {
            // 이렇게 해야 Disconnect 2번 해도 문제가 생기지 않는다.
            // 그런데 예제와는 다르게 현재 Disconnect 를 2번 호출해도 에러가 안나온다.
            // 네트워크 관련 라이브러리가 내부적으로 변경이 되어 이렇게 된 것 같다.
            // 일단 Disconnect() 를 멀티 스레드 환경에서 2번 호출해도 문제가 없게 하려면 이런 식으로 하면 된다는 것만 이해하자.
            if (Interlocked.Exchange(ref _disconnected, 1) == 1)
            {
                return;
            }
            OnDisconnected(_socket.RemoteEndPoint);
            _socket.Shutdown(SocketShutdown.Both);
            _socket.Close();
            Clear();
        }

        #region 통신관련 내부 메서드
        void RegisterSend()
        {
            if (_disconnected == 1)
            {
                return;
            }

            while (_sendQueue.Count > 0)
            {
                ArraySegment<byte> buff = _sendQueue.Dequeue();
                //_sendArgs.BufferList.Add(new ArraySegment<byte>(buff, 0, buff.Length)); // 이렇게 쓰면 안된다.
                _pendingList.Add(buff);
            }

            // 여기서 _sendArgs.Buffer나 BufferList 둘 중 하나는 null 이어야 한다. 안그러면 에러
            _sendArgs.BufferList = _pendingList; // 이렇게 list 로 만들어 준 후 BufferList 에 할당하는 방식으로 써야한다.

            try
            {
                bool pending = _socket.SendAsync(_sendArgs);
                if (pending == false)
                {
                    OnSendCompleted(null, _sendArgs);
                }

            }
            catch (Exception e)
            {
                Console.WriteLine($"{MethodBase.GetCurrentMethod().Name} Failed {e}");
            }
        }

        void OnSendCompleted(object sender, SocketAsyncEventArgs args)
        {
            // _sendQueue 에 여러 스레드가 접근할 수 있으므로 작업을 넣고 빼는 것과 관련해서 lock 이 필요.
            lock (_lock)
            {
                if (args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
                {
                    try
                    {
                        _sendArgs.BufferList = null;
                        _pendingList.Clear();

                        OnSend(_sendArgs.BytesTransferred);

                        if (_sendQueue.Count > 0)
                        {
                            RegisterSend();
                        }
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine($"{MethodBase.GetCurrentMethod().Name} Failed {e}");
                    }
                }
                else
                {
                    Disconnect();
                }
            }
        }

        void RegisterRecv()
        {
            if (_disconnected == 1)
            {
                return;
            }

            ArraySegment<byte> segment = _recvBuffer.WriteSegment;
            _recvArgs.SetBuffer(segment.Array, segment.Offset, segment.Count);

            try
            {
                bool pending = _socket.ReceiveAsync(_recvArgs);
                if (pending == false)
                {
                    OnRecvCompleted(null, _recvArgs);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"{MethodBase.GetCurrentMethod().Name} Failed {e}");
            }

        }

        void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
            {
                try
                {
                    // Write 커서 이동
                    if (_recvBuffer.OnWrite(args.BytesTransferred) == false)
                    {
                        Disconnect();
                        return;
                    }

                    // 컨텐츠 쪽으로 데이터를 넘겨주고 얼마나 처리했는지 받는다.
                    int processedBytes = OnRecv(_recvBuffer.ReadSegment);
                    if (processedBytes < 0 || _recvBuffer.DataSize < processedBytes)
                    {
                        Disconnect();
                        return;
                    }

                    // Read 커서 이동
                    if (_recvBuffer.OnRead(processedBytes) == false)
                    {
                        Disconnect();
                        return;
                    }

                    RegisterRecv();

                }
                catch (Exception e)
                {
                    // 방어 코드를 더 추가한 뒤에 여기로 넘어오면 Disconnect() 하는 것이 좋을 것 같다.
                    Console.WriteLine($"{MethodBase.GetCurrentMethod().Name} Failed {e}");
                }
            }
            else
            {
                Disconnect();
            }
        }
        #endregion
    }
}
