Rem 혹시나 이 배치파일이 작동하지 않는다면 python 및 pyinstaller 설치 여부와 
Rem pyinstaller의 시스템 환경 변수 설정을 확인한다.
@echo off
pushd %~dp0

pyinstaller --onefile PacketGenerator.py

Rem 해당 배치 파일을 실행하면 dist 폴더 내부에 PacketGenerator.exe 파일이 만들어진다.
Rem 그래서 이렇게 이동하는 과정이 필요하다.
MOVE .\dist\PacketGenerator.exe .\GenPackets.exe

Rem 경로 표시는 \나 / 모두 상관없이 가능하다.
set destPath="..\..\Common\protoc-21.12-win64\bin"
XCOPY /Y GenPackets.exe %destPath%
XCOPY /Y /I .\Templates %destPath%\Templates

Rem 불필요 폴더 및 파일 삭제
RD /S /Q .\build 
RD /S /Q .\dist
RD /S /Q .\PacketGenerator.spec
