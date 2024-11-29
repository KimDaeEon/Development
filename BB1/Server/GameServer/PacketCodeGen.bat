SET curPath=%cd%
ECHO Initial Path: %curPath%

SET projectPath=%1
ECHO Project Path: %projectPath%

SET relativeOutputPath=%2
ECHO RelativeOutPutPath Path: %relativeOutputPath%

:: 아래까지 해야지 전체 프로젝트 최상단 폴더 BB1에 도착
CD %projectPath%
CD ..
CD ..
SET rootPath=%cd%
ECHO Root Path: %rootPath%

SET outputPath=%rootPath%\%relativeOutputPath%
ECHO Output Path: %outputPath%

CD %rootPath%\Common\Packet
ECHO Current Path for Protocol Compilation: %cd%

protoc.exe -I=./ --csharp_out=%outputPath% ./Protocol.proto ./Enum.proto ./Struct.proto
::IF ERRORLEVEL 1 PAUSE

CD %rootPath%\Tools\PacketGenerator\bin
ECHO Current Path for Packet Generator: %cd%

START PacketGenerator.exe -o %2 -t %3

CD %curPath%
ECHO Returning to Initial Path: %cd%

EXIT