#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

/mnt/c/Program\ Files\ \(x86\)/Steam/steamapps/common/DayZ\ Tools/Bin/WorkDrive/WorkDrive.exe /Mount P \
  "$(wslvar USERPROFILE)\Documents\DayZData"


cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\FuelControl" "$(wslpath -w "$SCRIPT_DIR/..")"

cmd.exe /c mklink /J "P:\FuelControl" "$(wslpath -w "$SCRIPT_DIR/..")"

# Community Framework

cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\JM" "$(wslpath -w "$SCRIPT_DIR/../../CommunityFramework/JM")"

cmd.exe /c mklink /J "P:\JM" "$(wslpath -w "$SCRIPT_DIR/../../CommunityFramework/JM")"

cmd.exe /c mklink /J "P:\JM\COT" "$(wslpath -w "$SCRIPT_DIR/../../DayZ-CommunityOnlineTools/JM/COT")"

# RFFSHeli_Core
cmd.exe /c mklink /J "P:\RFFSHeli_Core" "$(wslpath -w "$SCRIPT_DIR/../../RFFSHeli_Core")"

#HypeTrain Core
cmd.exe /c mklink /J "P:\HypeTrain" "$(wslpath -w "$SCRIPT_DIR/../../HypeTrain")"

# Missions

cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\missions" "$(wslpath -w "$SCRIPT_DIR/missions")"
