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

        public int ReceviedDataSize { get { return _writePos - _readPos; } } 
        
        public int FreeSize { get { return  _buffer.Count - _writePos; } } 

        public RecvBuffer(int bufferSize)
        {
            _buffer = new ArraySegment<byte>(new byte[bufferSize], 0, bufferSize);
        }

        public ArraySegment<byte> ReadSegment
        {
            get { return new ArraySegment<byte>(_buffer.Array, _buffer.Offset + _readPos, ReceviedDataSize); }
        }

        public ArraySegment<byte> WriteSegment
        {
            get { return new ArraySegment<byte>(_buffer.Array, _buffer.Offset + _writePos, FreeSize); }
        }

        public void Clean()
        {
            int receivedDataSize = ReceviedDataSize;
            if (receivedDataSize == 0)
            {
                // 앞으로 읽어와야할 데이터가 없으면 커서 위치 리셋
                _readPos = _writePos = 0;
            }
            else
            {
                // 남은 데이터를 시작 위치로 복사, 커서 이동
                Array.Copy(_buffer.Array, _buffer.Offset + _readPos, _buffer.Array, _buffer.Offset, receivedDataSize);
                _readPos = 0;
                _writePos = receivedDataSize;
            }
        }

        public bool OnRead(int numOfBytes)
        {
            if (numOfBytes > ReceviedDataSize)
            {
                //Console.WriteLine("Error on "+ MethodBase.GetCurrentMethod().Name);
                return false;
            }
            _readPos += numOfBytes;
            return true;
        }

        public bool OnWrite(int numOfBytes)
        {
            if(numOfBytes > FreeSize)
            {
                //Console.WriteLine("Error on "+ MethodBase.GetCurrentMethod().Name);
                return false;
            }
            _writePos += numOfBytes;
            return true;
        }
    }
}
