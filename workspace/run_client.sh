#!/bin/bash


WIN_PROGRAMS_PATH=$(wslvar "programfiles(x86)")
WIN_DZ_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZ"
WIN_DZS_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZServer"

cmd.exe /c start /D "$WIN_DZ_PATH" DayZDiag_x64.exe -mod="P:\Mods\@CF;P:\Mods\@COT;P:\Mods\@FuelControl" -filePatching -connect=127.0.0.1 -port=2302 -window
