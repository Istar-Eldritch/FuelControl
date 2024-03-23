set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "WORKDRIVE_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkDrive"

rmdir "%DAYZ_PATH%\FuelControl" /s /q
rmdir "P:\FuelControl" /s /q

rmdir "%DAYZ_PATH%\HL2_IDs" /s /q
rmdir "P:\HL2_IDs" /s /q

@REM Community Framework

rmdir "P:\JM\COT" /s /q
rmdir "%DAYZ_PATH%\JM" /s /q
rmdir "P:\JM" /s /q


@REM RFFSHeli_Core
rmdir "P:\RFFSHeli_Core"

@REM HypeTrain Core
rmdir "P:\HypeTrain"

@REM Liquid Framework
cmd.exe /c rmdir "P:\rag" /s /q

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Dismount P
