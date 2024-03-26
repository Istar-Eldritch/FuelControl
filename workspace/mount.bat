#Liquid Framework
cmd.exe /c mkdir "P:\rag\rag_stuff"
cmd.exe /c mklink /J "P:\rag\rag_stuff\rag_liquid_framework" "$(wslpath -w "$SCRIPT_DIR/../../LiquidFramework")"


set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "WORKDRIVE_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkDrive"

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Mount P "%UserProfile%\Documents\DayZData"

mklink /J "%DAYZ_PATH%\FuelControl" "%SCRIPT_DIR%\..\mod"
mklink /J "P:\FuelControl" "%SCRIPT_DIR%\..\mod"
mklink /J "P:\Mods" "%UserProfile%\Documents\Mods"

@REM IE Framework
mkdir "%DAYZ_PATH%IE"
mkdir "P:\IE"
mklink /J "%DAYZ_PATH%IE\Framework" "%UserProfile%\Documents\DZ\IEFramework\mod"
mklink /J "P:\IE\Framework" "%UserProfile%\Documents\DZ\IEFramework\mod"

@REM Community Framework

mklink /J "%DAYZ_PATH%JM" "%UserProfile%\Documents\DZ\CommunityFramework\JM"
mklink /J "P:\JM" "%UserProfile%\Documents\DZ\CommunityFramework\JM"

@REM COT

mklink /J "%DAYZ_PATH%JM\COT" "%UserProfile%\Documents\DZ\DayZ-CommunityOnlineTools\JM\COT"
mklink /J "P:\JM\COT" "%UserProfile%\Documents\DZ\DayZ-CommunityOnlineTools\JM\COT"

@REM RFFSHeli_Core
mklink /J "%DAYZ_PATH%RFFSHeli_Core" "%UserProfile%\Documents\DZ\RFFSHeli_Core"
mklink /J "P:\RFFSHeli_Core" "%UserProfile%\Documents\DZ\RFFSHeli_Core"

@REM HypeTrain Core
mklink /J "%DAYZ_PATH%HypeTrain" "%UserProfile%\Documents\DZ\HypeTrain"
mklink /J "P:\HypeTrain" "%UserProfile%\Documents\DZ\HypeTrain"

@REM Liquid Framework
mkdir "P:\rag\rag_stuff"
mklink /J "P:\rag\rag_stuff\rag_liquid_framework" "%UserProfile%\Documents\DZ\LiquidFramework"