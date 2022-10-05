START/WAIT ../../PacketGenerator/bin/PacketGenerator.exe ../../PacketGenerator/PDL.xml
XCOPY /Y GeneratedPackets.cs "../../DummyClient/Packet"
XCOPY /Y GeneratedPackets.cs "../../../UnityMMOWithMyServer/UnityProj/MMOwithMyServer/Assets/Scripts/Network/Packet"
XCOPY /Y GeneratedPackets.cs "../../CSharpServer/Packet"

XCOPY /Y ClientPacketManager.cs "../../DummyClient/Packet"
XCOPY /Y ClientPacketManager.cs "../../../UnityMMOWithMyServer/UnityProj/MMOwithMyServer/Assets/Scripts/Network/Packet"
XCOPY /Y ServerPacketManager.cs "../../CSharpServer/Packet"