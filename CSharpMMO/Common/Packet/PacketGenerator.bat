START/WAIT ../../PacketGenerator/bin/PacketGenerator.exe ../../PacketGenerator/PDL.xml
XCOPY /Y GeneratedPackets.cs "../../DummyClient/Packet"
XCOPY /Y GeneratedPackets.cs "../../CSharpServer/Packet"
XCOPY /Y ClientPacketManager.cs "../../DummyClient/Packet"
XCOPY /Y ServerPacketManager.cs "../../CSharpServer/Packet"