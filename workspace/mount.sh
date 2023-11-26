#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

/mnt/c/Program\ Files\ \(x86\)/Steam/steamapps/common/DayZ\ Tools/Bin/WorkDrive/WorkDrive.exe /Mount P \
  "$(wslvar USERPROFILE)\Documents\DayZData"


cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\FuelControl" "$(wslpath -w "$SCRIPT_DIR/..")"

cmd.exe /c mklink /J "P:\FuelControl" "$(wslpath -w "$SCRIPT_DIR/..")"

# Community Framework

cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\JM" "$(wslpath -w "$SCRIPT_DIR/../../CommunityFramework/JM")"

cmd.exe /c mklink /J "P:\JM" "$(wslpath -w "$SCRIPT_DIR/../../CommunityFramework/JM")"

# Missions

cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\missions" "$(wslpath -w "$SCRIPT_DIR/missions")"
