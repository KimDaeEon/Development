using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices; // 추가

namespace ServerCore
{
    // TODO: 추후 웹 서버 상에 이거 기록하도록 바꾸는 코드로 해야할 수도 있음
    public static class Logger
    {
        [Conditional("DEBUG")]
        public static void LogDebug(string message, [CallerMemberName] string callerName = "")
        {
            Console.WriteLine($"[DEBUG] ({callerName}) {message}");
        }

        [Conditional("DEBUG")]
        public static void LogWarning(string message, [CallerMemberName] string callerName = "")
        {
            Console.WriteLine($"[WARNING] ({callerName}) {message}");
        }

        public static void LogError(string message, [CallerMemberName] string callerName = "")
        {
            Console.WriteLine($"[ERROR] ({callerName}) {message}");
        }
    }
}