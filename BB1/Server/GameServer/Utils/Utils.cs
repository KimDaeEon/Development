using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;

namespace GameServer
{
    public static class Utils
    {
        public static long TickCount { get { return System.Environment.TickCount64; } }

        public static string GetCurrentFunctionName()
        {
            string caller = new StackTrace().GetFrame(1)?.GetMethod()?.Name ?? "Unknown";
            return caller;
        }

        public static IPAddress GetLocalIP()
        {
            var ipHost = Dns.GetHostEntry(Dns.GetHostName());

            foreach (IPAddress ip in ipHost.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    return ip;
                }
            }

            return IPAddress.Loopback;
        }
    }
}
