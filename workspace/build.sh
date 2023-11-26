#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

WIN_PROGRAMS_PATH=$(wslvar "programfiles(x86)")
WIN_DZ_TOOL_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder"

WIN_FC_ADDONS_PATH="$(wslvar "userprofile")\Documents\DayZData\Mods\@FuelControl\addons"
FC_ADDONS_PATH=$(wslpath "$WIN_FC_ADDONS_PATH")

echo $FC_ADDONS_PATH
rm -fr "$FC_ADDONS_PATH"
mkdir -p "$FC_ADDONS_PATH"

cmd.exe /c start /D "$WIN_DZ_TOOL_PATH" AddonBuilder.exe "P:\FuelControl\scripts" "$WIN_FC_ADDONS_PATH" -project="P:\\" -clear

