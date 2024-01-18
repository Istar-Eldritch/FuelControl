#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

/mnt/c/Program\ Files\ \(x86\)/Steam/steamapps/common/DayZ\ Tools/Bin/WorkDrive/WorkDrive.exe /Mount P \
  "$(wslvar USERPROFILE)\Documents\DayZData"

# Missions

cmd.exe /c mklink /J "C:\Program Files (x86)\Steam\steamapps\common\DayZ\missions" "$(wslpath -w "$SCRIPT_DIR/missions")"
