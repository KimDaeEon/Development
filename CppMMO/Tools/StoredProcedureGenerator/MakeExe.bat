Rem Ȥ�ó� �� ��ġ������ �۵����� �ʴ´ٸ� python �� pyinstaller ��ġ ���ο� 
Rem pyinstaller�� �ý��� ȯ�� ���� ������ Ȯ���Ѵ�.
@echo off
pushd %~dp0

pyinstaller --onefile StoredProcedureGenerator.py

Rem �ش� ��ġ ������ �����ϸ� dist ���� ���ο� StoredProcedureGenerator.exe ������ ���������.
Rem �׷��� �̷��� �̵��ϴ� ������ �ʿ��ϴ�.
MOVE .\dist\StoredProcedureGenerator.exe .\GenStoredProcedures.exe

Rem ��� ǥ�ô� \�� / ��� ������� �����ϴ�.
set destPath="..\..\Common\storedProcedures"
XCOPY /Y GenStoredProcedures.exe %destPath%
XCOPY /Y /I .\Templates %destPath%\Templates

Rem ���ʿ� ���� �� ���� ����
RD /S /Q .\build 
RD /S /Q .\dist
RD /S /Q .\StoredProcedureGenerator.spec
