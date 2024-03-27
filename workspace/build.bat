set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "ADDON_BUILDER_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder"

set "MOD_PATH=P:\Mods\@FuelControl"
rmdir /s "%MOD_PATH%"
md "%MOD_PATH%/addons"
xcopy /s "%SCRIPT_DIR%/../keys" "%MOD_PATH%/keys"
xcopy /s "%SCRIPT_DIR%/../profiles" "%MOD_PATH%/profiles"
copy "%SCRIPT_DIR%/../LICENSE" "%MOD_PATH%/LICENSE"
copy "%SCRIPT_DIR%/../README.md" "%MOD_PATH%/README.md"
copy "%SCRIPT_DIR%/../configuration.md" "%MOD_PATH%/configuration.md"
copy "%SCRIPT_DIR%/../mod.cpp" "%MOD_PATH%/mod.cpp"

start /wait /D "%ADDON_BUILDER_PATH%" AddonBuilder.exe "P:\FuelControl" "%MOD_PATH%\addons" -project="P:" -prefix="FuelControl" -clear -binarizeFullLogs -sign="%UserProfile%\Documents\Keys\IstarEldritch.biprivatekey"