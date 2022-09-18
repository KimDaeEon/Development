using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    public class SendBufferHelper
    {
        public static ThreadLocal<SendBuffer> CurrentBuffer = new ThreadLocal<SendBuffer>(() => { return null; });

        public static int ChunkSize { get; set; } = 4096 * 100;

        public static ArraySegment<byte> Open(int reserveSize)
        {
            if(CurrentBuffer.Value == null)
            {
                CurrentBuffer.Value = new SendBuffer(ChunkSize);
            }

            // TODO: 일단 간단하게 이렇게 했는데, 다 쓰면 이 버퍼를 재사용할 수 있도록 개선하면 좋을 것 같다.
            if(CurrentBuffer.Value.FreeSize < reserveSize)
            {
                CurrentBuffer.Value = new SendBuffer(ChunkSize);
            }

            return CurrentBuffer.Value.Open(reserveSize);
        }

        public static ArraySegment<byte> Close(int usedSize)
        {
            return CurrentBuffer.Value.Close(usedSize);
        }
    }

    // SendBuffer 는 RecvBuffer 와 다르게 Session 하나에 종속적이여서는 안된다.
    // 왜냐하면 플레이어가 움직인다고 했을 때에 같은 내용의 패킷을 여러 세션에서 보낼 것인데,
    // 세션 내부에 SendBuffer 가 종속적이면 같은 내용을 보내려고 SendBuffer 가 여러개 만들어지는 상황이 발생한다.

    // 그렇기에 SendBuffer 를 Session 외부에 두고, 그 SendBuffer 를 받아서 send 하도록 하는 것이 
    // 성능상으로 큰 이점이 있다.

    // 한 번 만들어진 SendBuffer 는 여러 세션에서 사용될 수 있으므로, buffer 내부의 내용을 초기화해서 재사용하지 않고,
    // 그 때 그 때에 다시 만들어서 사용한다.
    public class SendBuffer
    {
        byte[] _buffer;
        int _usedSize = 0;

        public int FreeSize { get { return _buffer.Length - _usedSize; } }

        public SendBuffer(int chunkSize)
        {
            _buffer = new byte[chunkSize];
        }

        public ArraySegment<byte> Open(int reserveSize)
        {
            if (reserveSize > FreeSize)
            {
                return null;
            }

            return new ArraySegment<byte>(_buffer, _usedSize, reserveSize);
        }

        // _usedSize, usedSize 구분해서 잘 보기.
        public ArraySegment<byte> Close(int usedSize)
        {
            ArraySegment<byte> segment = new ArraySegment<byte>(_buffer, _usedSize, usedSize);
            _usedSize += usedSize;

            return segment;
        }
    }
}
