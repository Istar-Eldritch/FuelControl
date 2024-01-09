#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

WIN_PROGRAMS_PATH=$(wslvar "programfiles(x86)")
WIN_DZ_TOOL_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder"

WIN_FC_PATH="$(wslvar "userprofile")\Documents\DayZData\Mods\@FuelControl"
FC_PATH=$(wslpath "$WIN_FC_PATH")

echo $FC_PATH
rm -fr "$FC_PATH"
mkdir -p "$FC_PATH/addons"
cp -r "$SCRIPT_DIR/../keys" "$FC_PATH/keys"
cp -r "$SCRIPT_DIR/../profiles" "$FC_PATH/profiles"
cp -r "$SCRIPT_DIR/../LICENSE" "$FC_PATH/LICENSE"
cp -r "$SCRIPT_DIR/../README.md" "$FC_PATH/README.md"
cp -r "$SCRIPT_DIR/../commands.md" "$FC_PATH/commands.md"
cp -r "$SCRIPT_DIR/../configuration.md" "$FC_PATH/configuration.md"
cp -r "$SCRIPT_DIR/../mod.cpp" "$FC_PATH/mod.cpp"

cmd.exe /c start /D "$WIN_DZ_TOOL_PATH" AddonBuilder.exe "P:\FuelControl\scripts" "$WIN_FC_PATH\addons" -project="P:" -prefix="FuelControl" -clear -binarizeFullLogs -sign="$(wslvar "userprofile")\Documents\Keys\IstarEldritch.biprivatekey"
cmd.exe /c start /D "$WIN_DZ_TOOL_PATH" AddonBuilder.exe "P:\FuelControl\GUI" "$WIN_FC_PATH\addons" -project="P:" -prefix="FuelControl" -clear -binarizeFullLogs -sign="$(wslvar "userprofile")\Documents\Keys\IstarEldritch.biprivatekey"
