@echo off
echo Please run as administrator.
%~dp0procdump.exe -i %~dp0 -mm
pause
