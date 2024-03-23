set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "WORKBENCH_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkBench"

start /D "%WORKBENCH_PATH%" workbenchApp.exe "-profiles=%SCRIPT_DIR%\Workbench" "-mod=P:\JM\CF\Scripts;P:\JM\COT\Scripts;P:\FuelControl"
