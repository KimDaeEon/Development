using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public class RecvBuffer
    {
        ArraySegment<byte> _buffer;
        int _readPos = 0;
        int _writePos = 0;

        // 지금 들어온 데이터 크기
        public int DataSize { get { return _writePos - _readPos; } } 
        
        // 현재 버퍼에 남은 크기
        public int FreeSize { get { return  _buffer.Count - _writePos; } } 

        public RecvBuffer(int bufferSize)
        {
            _buffer = new ArraySegment<byte>(new byte[bufferSize], 0, bufferSize);
        }

        public ArraySegment<byte> ReadSegment
        {
            get { return new ArraySegment<byte>(_buffer.Array, _buffer.Offset + _readPos, DataSize); }
        }

        public ArraySegment<byte> WriteSegment
        {
            get { return new ArraySegment<byte>(_buffer.Array, _buffer.Offset + _writePos, FreeSize); }
        }

        public void Clean()
        {
            int dataSize = DataSize;
            if (dataSize == 0)
            {
                // 앞으로 읽어와야할 데이터가 없으면 커서 위치 리셋
                _readPos = _writePos = 0;
            }
            else
            {
                // 남은 데이터를 시작 위치로 복사, 커서 이동
                Array.Copy(_buffer.Array, _buffer.Offset + _readPos, _buffer.Array, _buffer.Offset, dataSize);
                _readPos = 0;
                _writePos = dataSize;
            }
        }

        public bool OnRead(int numOfBytes)
        {
            if (numOfBytes > DataSize)
            {
                Console.WriteLine("Error on "+ MethodBase.GetCurrentMethod().Name);
                return false;
            }
            _readPos += numOfBytes;
            return true;
        }

        public bool OnWrite(int numOfBytes)
        {
            if(numOfBytes > FreeSize)
            {
                Console.WriteLine("Error on "+ MethodBase.GetCurrentMethod().Name);
                return false;
            }
            _writePos += numOfBytes;
            return true;
        }
    }
}
