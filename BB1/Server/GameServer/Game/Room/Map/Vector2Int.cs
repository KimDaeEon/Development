using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace GameServer
{
    // 2D 좌표 용도로 사용될 객체
    public struct Vector2Int
    {
        public int x;
        public int y;

        public Vector2Int(int x, int y) { this.x = x; this.y = y; }

        public static Vector2Int Up { get { return new Vector2Int(0, 1); } }
        public static Vector2Int Down { get { return new Vector2Int(0, -1); } }
        public static Vector2Int Left { get { return new Vector2Int(-1, 0); } }
        public static Vector2Int Right { get { return new Vector2Int(1, 0); } }
        public static Vector2Int Zero { get { return new Vector2Int(0, 0); } }

        public static Vector2Int operator *(Vector2Int v, int c)
        {
            return new Vector2Int(v.x * c, v.y * c);
        }

        public static Vector2Int operator +(Vector2Int a, Vector2Int b)
        {
            return new Vector2Int(a.x + b.x, a.y + b.y);
        }

        public static Vector2Int operator -(Vector2Int a, Vector2Int b)
        {
            return new Vector2Int(a.x - b.x, a.y - b.y);
        }

        public static bool operator ==(Vector2Int lhs, Vector2Int rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        public static bool operator !=(Vector2Int lhs, Vector2Int rhs)
        {
            return !(lhs == rhs);
        }

        public override bool Equals(object obj)
        {
            return (Vector2Int)obj == this;
        }

        public override int GetHashCode()
        {
            return (x << 16) ^ y;
        }

        public float magnitude { get { return (float)Math.Sqrt(sqrMagnitude); } }
        public int sqrMagnitude { get { return (x * x + y * y); } }
        public int cellDistFromZero { get { return Math.Abs(x) + Math.Abs(y); } }
    }
}
