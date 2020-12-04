@echo off

setlocal

set _python=python.exe
set _script="%~dp0%makePatch.py"

call %_python% %_script% %1 %2 %3 %4 %5 %6 %7 %8
goto :eof

endlocal

:eof
