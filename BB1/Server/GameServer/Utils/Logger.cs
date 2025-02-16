using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // TODO: 추후 웹 서버 상에 이거 기록하도록 바꾸는 코드로 해야할 수도 있음
    internal static class Logger
    {
        [Conditional("DEBUG")]
        public static void LogDebug(string message)
        {
            Logger.LogDebug($"[DEBUG] {message}");
        }

        [Conditional("DEBUG")]
        public static void LogWarning(string message)
        {
            Logger.LogDebug($"[WARNING] {message}");
        }

        public static void LogError(string message)
        {
            Logger.LogDebug($"[ERROR] {message}");
        }
    }
}