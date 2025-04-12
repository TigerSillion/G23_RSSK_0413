rem === For setting CC-RL environment ====
rem ** Please update the "InstallDirectory" to the CC-RL installed directory.
set INSTDIR_CCRL=InstallDirectory
if "%INSTDIR_CCRL%"=="" goto TOEXIT
if not exist "%INSTDIR_CCRL%" goto TOEXIT
set PATH=%INSTDIR_CCRL%\bin;%PATH%
:TOEXIT
