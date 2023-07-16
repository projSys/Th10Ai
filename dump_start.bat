
set exeName=Th10Ai.exe
::set exeName=%1

set dumpFolder="%~dp0CrashDumps"
::set dumpFolder="%LOCALAPPDATA%\CrashDumps"

set keyName="HKLM\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\%exeName%"

reg add %keyName% /f
reg add %keyName% /v DumpFolder /t REG_EXPAND_SZ /d %dumpFolder% /f
reg add %keyName% /v DumpCount /t REG_DWORD /d 10 /f
reg add %keyName% /v DumpType /t REG_DWORD /d 1 /f

sc config WerSvc start= auto
sc start WerSvc

pause
