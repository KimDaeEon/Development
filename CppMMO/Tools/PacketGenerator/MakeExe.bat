Rem Ȥ�ó� �� ��ġ������ �۵����� �ʴ´ٸ� python �� pyinstaller ��ġ ���ο� 
Rem pyinstaller�� �ý��� ȯ�� ���� ������ Ȯ���Ѵ�.
@echo off
pushd %~dp0

pyinstaller --onefile PacketGenerator.py

Rem �ش� ��ġ ������ �����ϸ� dist ���� ���ο� PacketGenerator.exe ������ ���������.
Rem �׷��� �̷��� �̵��ϴ� ������ �ʿ��ϴ�.
MOVE .\dist\PacketGenerator.exe .\GenPackets.exe

Rem ��� ǥ�ô� \�� / ��� ������� �����ϴ�.
set destPath="..\..\Common\protoc-21.12-win64\bin"
XCOPY /Y GenPackets.exe %destPath%
XCOPY /Y /I .\Templates %destPath%\Templates

Rem ���ʿ� ���� �� ���� ����
RD /S /Q .\build 
RD /S /Q .\dist
RD /S /Q .\PacketGenerator.spec
