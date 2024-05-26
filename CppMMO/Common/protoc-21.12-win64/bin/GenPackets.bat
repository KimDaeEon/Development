@echo off
pushd %~dp0

protoc -I=./ --cpp_out=./ ./Enum.proto 
protoc -I=./ --cpp_out=./ ./Struct.proto 
protoc -I=./ --cpp_out=./ ./Protocol.proto 

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=C_ --send=S_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=S_ --send=C_

IF ERRORLEVEL 1 PAUSE

SET gameServerPath="..\..\..\GameServer"
SET dummyClientPath="..\..\..\DummyClient"
SET unrealClientPath="..\..\..\MMOClient\Source\MMOClient\Network"

XCOPY /Y Enum.pb.h %gameServerPath%
XCOPY /Y Enum.pb.cc %gameServerPath%
XCOPY /Y Struct.pb.h %gameServerPath%
XCOPY /Y Struct.pb.cc %gameServerPath%
XCOPY /Y Protocol.pb.h %gameServerPath%
XCOPY /Y Protocol.pb.cc %gameServerPath%
XCOPY /Y ClientPacketHandler.h %gameServerPath%

XCOPY /Y Enum.pb.h %dummyClientPath%
XCOPY /Y Enum.pb.cc %dummyClientPath%
XCOPY /Y Struct.pb.h %dummyClientPath%
XCOPY /Y Struct.pb.cc %dummyClientPath%
XCOPY /Y Protocol.pb.h %dummyClientPath%
XCOPY /Y Protocol.pb.cc %dummyClientPath%
XCOPY /Y ServerPacketHandler.h %dummyClientPath%

XCOPY /Y Enum.pb.h %unrealClientPath%
XCOPY /Y Enum.pb.cc %unrealClientPath%
XCOPY /Y Struct.pb.h %unrealClientPath%
XCOPY /Y Struct.pb.cc %unrealClientPath%
XCOPY /Y Protocol.pb.h %unrealClientPath%
XCOPY /Y Protocol.pb.cc %unrealClientPath%
XCOPY /Y ServerPacketHandler.h %unrealClientPath%
