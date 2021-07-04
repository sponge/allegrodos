@ECHO OFF
:loop

cd c:\
call launch.exe

IF ERRORLEVEL 0 SET LAUNCH=0
IF ERRORLEVEL 1 SET LAUNCH=1
IF ERRORLEVEL 2 SET LAUNCH=2
IF ERRORLEVEL 3 SET LAUNCH=3
IF ERRORLEVEL 4 SET LAUNCH=4
IF ERRORLEVEL 5 SET LAUNCH=5
IF ERRORLEVEL 6 SET LAUNCH=6

if %LAUNCH% == 0 GOTO done
if %LAUNCH% == 1 GOTO launch1
if %LAUNCH% == 2 GOTO launch2
if %LAUNCH% == 3 GOTO launch3
if %LAUNCH% == 4 GOTO launch4
if %LAUNCH% == 5 GOTO launch5
if %LAUNCH% == 6 GOTO launch6

:launch1
  echo launch 1
  pause
  GOTO loop

:launch2
  echo launch 2
  pause
  GOTO loop

:launch3
  echo launch 3
  pause
  GOTO loop

:launch4
  echo launch 4
  pause
  GOTO loop

:launch5
  echo launch 5
  pause
  GOTO loop

:launch6
  cd keen6
  keen6c.exe
  GOTO loop

:done
  exit