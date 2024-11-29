using System;
using System.IO;
using System.Reflection;
using CommandLine;

namespace PacketGenerator
{
    // 아래 타입과 연동되어서 패킷 핸들러 파일명과 코드가 생성되어야 하는 위치가 달라진다.
    public enum ProgramType
    {
        None = -1,
        Client = 0,
        GameServer = 1,
    }

    public class CommandLineOptions
    {
        [Option('o', "outputPath", Required = true, HelpText = "Input output path. ex) Server/GameServer/Packet/Generated/ or Client/Assets/Scripts/Packet/Generated/ (root = CSharpMMO 경로부터)")]
        public string outputPath { get; set; }
        [Option('t', "programType", Required = true, HelpText = "Client = 0, GameServer = 1")]
        public int programType { get; set; }
    }


    class Program
    {
        // PacketManager 에 등록되는 핸들러 관련 코드들
        static string gameServerPacketManager;
        static string clientPacketManager;

        // 메세지 ID 자동화를 위한 변수
        static string gameServerMsgIdList;
        static string clientMsgIdList;
        static int protocolIdIssuer = 1;

        // 자동화 패킷 처리 코드 생성 경로
        static string outputPath = "";
        static ProgramType programType = ProgramType.None;

        static void SaveCommandLineOptions(CommandLineOptions opts)
        {
            outputPath = opts.outputPath;
            programType = (ProgramType)opts.programType;
        }
        public static void ParsePacket(string name)
        {
            // 추후에 새로운 프로토콜이 추가되면 여기에 추가
            if (name.StartsWith("S_")) // GameServer -> Client
            {
                clientPacketManager += string.Format(PacketManagerCodeGenForamt.packetHandlerFormat, name);
                gameServerMsgIdList += string.Format(PacketManagerCodeGenForamt.msgIdFormat, name, protocolIdIssuer);
                clientMsgIdList += string.Format(PacketManagerCodeGenForamt.msgIdFormat, name, protocolIdIssuer);

                protocolIdIssuer++;
            }
            else if (name.StartsWith("C_")) // Client -> GameServer
            {
                gameServerPacketManager += string.Format(PacketManagerCodeGenForamt.packetHandlerFormat, name);
                clientMsgIdList += string.Format(PacketManagerCodeGenForamt.msgIdFormat, name, protocolIdIssuer);
                gameServerMsgIdList += string.Format(PacketManagerCodeGenForamt.msgIdFormat, name, protocolIdIssuer);

                protocolIdIssuer++;
            }
        }

        static void Main(string[] args)
        {
            string rootDirPath = "../../../";
            string protoFilePath = rootDirPath + "Common/Packet/Protocol.proto";

            CommandLine.Parser.Default
                .ParseArguments<CommandLineOptions>(args)
                .WithParsed(SaveCommandLineOptions);

            foreach (string line in File.ReadAllLines(protoFilePath))
            {
                string[] names = line.Split(" ");
                if (names.Length == 0)
                    continue;

                if (!names[0].StartsWith("message"))
                    continue;

                ParsePacket(names[1]);
            }

            if (programType == ProgramType.Client)
            {
                string clientManagerText = string.Format(PacketManagerCodeGenForamt.managerFormat, clientMsgIdList, clientPacketManager);
                File.WriteAllText(rootDirPath + outputPath + "ClientPacketManager.cs", clientManagerText);
            }
            else if (programType == ProgramType.GameServer)
            {
                string gameServerManagerText = string.Format(PacketManagerCodeGenForamt.managerFormat, gameServerMsgIdList, gameServerPacketManager);
                File.WriteAllText(rootDirPath + outputPath + "GameServerPacketManager.cs", gameServerManagerText);
            }
        }
    }
}
