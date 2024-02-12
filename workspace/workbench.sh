#!/bin/bash

WIN_PROGRAMS_PATH=$(wslvar "programfiles(x86)")
WIN_DZ_TOOLS_PATH="$WIN_PROGRAMS_PATH\Steam\steamapps\common\DayZ Tools\Bin\Workbench"

cmd.exe /c start /D "${WIN_DZ_TOOLS_PATH}" workbenchApp.exe -mod="P:\JM\CF\Scripts;P:\JM\COT\Scripts;P:\rag\rag_stuff\rag_liquid_framework;P:\RFFSHeli_Core;P:\RFWC_Core;P:\HypeTrain\core;P:\FuelControl\scripts"
