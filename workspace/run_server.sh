#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


WIN_PROGRAMS_PATH=$(wslvar "programfiles(x86)")
WIN_DZ_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZ"
WIN_DZS_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZServer"

cmd.exe /c start /D "$WIN_DZ_PATH" DayZDiag_x64.exe -mission="$WIN_DZS_PATH\mpmissions\dayzOffline.chernarusplus" -mod="P:\Mods\@CF;P:\Mods\@COT;P:\Mods\@FuelControl;" -filePatching -server -profiles=profiles -config="$(wslupath "$SCRIPT_DIR/serverDZ.cfg")"
