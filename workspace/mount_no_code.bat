set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "WORKDRIVE_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkDrive"

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Mount P "%UserProfile%\Documents\DayZData"

mklink /J "P:\Mods" "%UserProfile%\Documents\Mods"