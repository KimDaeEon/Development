set src_dir=../../../PacketGenerator
protoc -I=%src_dir% --csharp_out=./ %src_dir%/Protocol.proto
IF ERRORLEVEL 1 PAUSE

START/WAIT %src_dir%/bin/PacketGenerator.exe %src_dir%/Protocol.proto
XCOPY /Y Protocol.cs "../../../DummyClient/Packet"
XCOPY /Y Protocol.cs "../../../../UnityMMOWithMyServer/UnityProj/MMOwithMyServer/Assets/Scripts/Network/Packet"
XCOPY /Y Protocol.cs "../../../CSharpServer/Packet"

XCOPY /Y ClientPacketManager.cs "../../../DummyClient/Packet"
XCOPY /Y ClientPacketManager.cs "../../../../UnityMMOWithMyServer/UnityProj/MMOwithMyServer/Assets/Scripts/Network/Packet"
XCOPY /Y ServerPacketManager.cs "../../../CSharpServer/Packet"