set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "MODS=%DAYZ_PATH%!Workshop"

start /D "%DAYZ_PATH%" DayZDiag_x64.exe "-mod=%MODS%\@CF;%MODS%\@Community-Online-Tools;%MODS%\@RaG_Liquid_Framework;%MODS%\@RedFalconHeliz;%MODS%\@HypeTrain;P:\Mods\@FuelControl" "-name=Istar Eldritch" -filePatching -connect=127.0.0.1 -port=2302 -window
