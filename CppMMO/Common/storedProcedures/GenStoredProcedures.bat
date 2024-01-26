@echo off
pushd %~dp0

SET gameServerPath=..\..\GameServer

GenStoredProcedures.exe --path=%gameServerPath%/GameDB.xml --output=GeneratedStoredProcedures.h

XCOPY /Y GeneratedStoredProcedures.h %gameServerPath%
