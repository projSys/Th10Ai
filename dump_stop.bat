
set exeName=Th10Ai.exe
::set exeName=%1

set keyName="HKLM\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\%exeName%"
::set keyName="HKLM\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps"

reg delete %keyName% /f

sc config WerSvc start= demand
sc stop WerSvc

pause
