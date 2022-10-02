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

        static string generatedPackets;
        static ushort packetId = 0;
        static string packetEnums;
        static void Main(string[] args)
        {
            string pdlPath = "../PDL.xml";

            if(args.Length >= 1)
            {
                pdlPath = args[0];
            }

            XmlReaderSettings settings = new XmlReaderSettings()
            {
                IgnoreComments = true,
                IgnoreWhitespace = true
            };

            using (XmlReader reader = XmlReader.Create(pdlPath, settings))
            {
                reader.MoveToContent();
                while (reader.Read())
                {
                    if (reader.Depth == 1 && reader.NodeType == XmlNodeType.Element)
                    {
                        ParsePacket(reader);
                    }
                    Console.WriteLine(reader.Name + " " + reader["name"]);
                }

                string fileText = string.Format(PacketFormat.fileFormat, packetEnums, generatedPackets);
                File.WriteAllText("GeneratedPackets.cs", fileText);
               
                string serverManagerText = string.Format(PacketFormat.managerFormat, registeredHandlersForServer);
                File.WriteAllText("ServerPacketManager.cs", serverManagerText);
                
                string clientManagerText = string.Format(PacketFormat.managerFormat, registeredHandlersForClient);
                File.WriteAllText("ClientPacketManager.cs", clientManagerText);
            }
        }


        public static void ParsePacket(XmlReader reader)
        {
            if (reader.NodeType == XmlNodeType.EndElement)
            {
                return;
            }

            // 현재 읽고 있는 item 의 이름이 packet 이 아니라면 파싱하지 않는다.
            if (reader.Name.ToLower() != "packet")
            {
                Console.WriteLine("Packet node name is invalid.");
                return;
            }

            string packetName = reader["name"];
            if (string.IsNullOrEmpty(packetName))
            {
                Console.WriteLine("Packet has no name.");
                return;
            }

            Tuple<string, string, string> tuple = ParseMembers(reader);
            generatedPackets += string.Format(PacketFormat.packetFormat, packetName, tuple.Item1, tuple.Item2, tuple.Item3);
            packetEnums += string.Format(PacketFormat.packetEnumFormat, packetName, ++packetId) + Environment.NewLine + "\t";

            if(packetName.StartsWith("C_") || packetName.StartsWith("c_"))
            {
                registeredHandlersForServer += string.Format(PacketFormat.registeredHandlerFormat, packetName) + Environment.NewLine;
            }
            else
            {
                registeredHandlersForClient += string.Format(PacketFormat.registeredHandlerFormat, packetName) + Environment.NewLine;
            }
        }

        public static Tuple<string, string, string> ParseMembers(XmlReader reader)
        {
            string packetName = reader["name"];

            string memberCode = ""; // 클래스 멤버 함수나 변수 관련 코드
            string readCode = "";
            string writeCode = "";

            // 현재 파싱되어야 하는 아이템의 depth 는 패킷 다음에 있는 요소이므로 reader.Depth + 1
            int depth = reader.Depth + 1;
            while (reader.Read())
            {
                if (reader.Depth != depth)
                {
                    break;
                }

                string memberName = reader["name"];
                if (string.IsNullOrEmpty(memberName))
                {
                    Console.WriteLine("Member without name");
                    return null;
                }

                if (string.IsNullOrEmpty(memberCode) == false)
                {
                    memberCode += Environment.NewLine;
                }

                if (string.IsNullOrEmpty(readCode) == false)
                {
                    readCode += Environment.NewLine;
                }

                if (string.IsNullOrEmpty(writeCode) == false)
                {
                    writeCode += Environment.NewLine;
                }

                string memberType = reader.Name.ToLower();
                switch (memberType)
                {
                    case "byte":
                    case "sbyte":
                        memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
                        readCode += string.Format(PacketFormat.readByteFormat, memberName, memberType);
                        writeCode += string.Format(PacketFormat.writeByteFormat, memberName, memberType);
                        break;

                    case "bool":
                    case "short":
                    case "ushort":
                    case "int":
                    case "long":
                    case "float":
                    case "double":
                        memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
                        readCode += string.Format(PacketFormat.readFormat, memberName, ToMemberType(memberType), memberType);
                        writeCode += string.Format(PacketFormat.writeFormat, memberName, memberType);
                        break;

                    case "string":
                        memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
                        readCode += string.Format(PacketFormat.readStringFormat, memberName);
                        writeCode += string.Format(PacketFormat.writeStringFormat, memberName);
                        break;

                    case "list":
                        Tuple<string, string, string> tuple = ParseList(reader);
                        memberCode += tuple.Item1;
                        readCode += tuple.Item2;
                        writeCode += tuple.Item3;
                        break;

                    default:
                        break;
                }
            } // while(reader.Read())

            memberCode = memberCode.Replace("\n", "\n\t");
            readCode = readCode.Replace("\n", "\n\t\t");
            writeCode = writeCode.Replace("\n", "\n\t\t");

            return new Tuple<string, string, string>(memberCode, readCode, writeCode);
        }

        public static Tuple<string, string, string> ParseList(XmlReader reader)
        {
            string listName = reader["name"];
            if (string.IsNullOrEmpty(listName))
            {
                Console.WriteLine("list has no name.");
                return null;
            }

            Tuple<string, string, string> tuple = ParseMembers(reader);

            string memberCode = string.Format(PacketFormat.memberListFormat,
                ToUpperFirstChar(listName),
                ToLowerFirstChar(listName),
                tuple.Item1,
                tuple.Item2,
                tuple.Item3);

            string readCode = string.Format(PacketFormat.readListFormat,
                ToUpperFirstChar(listName),
                ToLowerFirstChar(listName)
                );

            string writeCode = string.Format(PacketFormat.writeListFormat,
                ToUpperFirstChar(listName),
                ToLowerFirstChar(listName)
                );


            return new Tuple<string, string, string> (memberCode, readCode, writeCode);
        }

        public static string ToUpperFirstChar(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return null;
            }

            return input[0].ToString().ToUpper() + input.Substring(1);
        }

        public static string ToLowerFirstChar(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return null;
            }

            return input[0].ToString().ToLower() + input.Substring(1);
        }

        public static string ToMemberType(string memberType)
        {
            // byte 를 읽어서 특정 타입으로 바꾸는 함수의 이름을 여기서 만들어준다.
            switch (memberType)
            {
                // byte 는 기본 데이터형이므로 여기선 필요없다.
                case "bool":
                    return "ToBoolean";
                case "short":
                    return "ToInt16";
                case "ushort":
                    return "ToUInt16";
                case "int":
                    return "ToInt32";
                case "long":
                    return "ToInt64";
                case "float":
                    return "ToSingle";
                case "double":
                    return "ToDouble";
                default:
                    return "";
            }
        }

    }
}
