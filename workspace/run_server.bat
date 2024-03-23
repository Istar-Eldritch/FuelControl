set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "DAYZS_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZServer\"
set "MODS=%DAYZ_PATH%!Workshop"

if not exist "%SCRIPT_DIR%\mission" (
    mkdir "%SCRIPT_DIR%\mission"
    xcopy /s "%DAYZS_PATH%\mpmissions\dayzOffline.chernarusplus" "%SCRIPT_DIR%\mission\dayzOffline.chernarusplus"
)

start /D "%DAYZ_PATH%" DayZDiag_x64.exe "-mission=%SCRIPT_DIR%\mission\dayzOffline.chernarusplus" "-mod=%MODS%\@CF;%MODS%\@Community-Online-Tools;%MODS%\@RaG_Liquid_Framework;%MODS%\@RedFalconHeliz;%MODS%\@HypeTrain;P:\Mods\@FuelControl" -server -filePatching "-profiles=%SCRIPT_DIR%\profiles" "-config=%SCRIPT_DIR%\serverDZ.cfg"
