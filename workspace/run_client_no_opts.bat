set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"

start /D "%DAYZ_PATH%" DayZDiag_x64.exe "-mod=P:\Mods\@CF;P:\Mods\@FuelControl" "-name=Istar Eldritch" -filePatching -connect=127.0.0.1 -port=2302 -window
