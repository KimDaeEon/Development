pushd %~dp0

protoc -I=./ --cpp_out=./ ./Enum.proto 
protoc -I=./ --cpp_out=./ ./Struct.proto 
protoc -I=./ --cpp_out=./ ./Protocol.proto 

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=C_ --send=S_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=S_ --send=C_

IF ERRORLEVEL 1 PAUSE

SET gameServerPath="..\..\..\GameServer"
SET dummyClientPath="..\..\..\DummyClient"

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
