using System;
using System.IO;
using System.Xml;

namespace PacketGenerator
{
    class Program
    {
        // PacketManager 에 등록되는 핸들러 관련 코드들
        static string registeredHandlersForServer;
        static string registeredHandlersForClient;

        static void Main(string[] args)
        {
            string protoFilePath = "../Protocol.proto";

            if (args.Length >= 1)
            {
                // cmd 창에서 경로 받는 경우
                protoFilePath = args[0];
            }

            bool startParsing = false;
            foreach (string line in File.ReadAllLines(protoFilePath))
            {
                if (!startParsing && line.Contains("enum MsgId"))
                {
                    startParsing = true;
                    continue;
                }

                if (!startParsing)
                {
                    continue;
                }

                if (line.Contains("}"))
                {
                    break;
                }

                string[] names = line.Trim().Split(" =");
                if (names.Length == 0)
                {
                    continue;
                }

                string name = names[0];
                
                // 클라이언트용 패킷인 경우 (서버에서 보내주는 패킷인 경우)
                if (name.StartsWith("S_"))
                {
                    string[] words = name.Split("_");

                    string msgName = "";
                    foreach (string word in words)
                        msgName += FirstCharToUpper(word);

                    string packetName = $"S_{msgName.Substring(1)}";
                    registeredHandlersForClient += string.Format(PacketFormat.registeredHandlerFormat, msgName, packetName);
                }

                // 서버용 패킷인 경우 (클라이언트에서 보내주는 패킷인 경우)
                else if (name.StartsWith("C_"))
                {
                    string[] words = name.Split("_");

                    string msgName = "";
                    foreach (string word in words)
                        msgName += FirstCharToUpper(word);

                    string packetName = $"C_{msgName.Substring(1)}";
                    registeredHandlersForServer += string.Format(PacketFormat.registeredHandlerFormat, msgName, packetName);
                }
            }

            string clientManagerText = string.Format(PacketFormat.managerFormat, registeredHandlersForClient);
            File.WriteAllText("ClientPacketManager.cs", clientManagerText);
            string serverManagerText = string.Format(PacketFormat.managerFormat, registeredHandlersForServer);
            File.WriteAllText("ServerPacketManager.cs", serverManagerText);
        }

        public static string FirstCharToUpper(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return "";
            }

            return input[0].ToString().ToUpper() + input.Substring(1).ToLower();
        }
    }
}
