#!/bin/bash

cmd.exe /c rmdir "C:\Program Files (x86)\Steam\steamapps\common\DayZ\FuelControl"
cmd.exe /c rmdir "P:\FuelControl"

# Community Framework

cmd.exe /c rmdir "P:\JM\COT"
cmd.exe /c rmdir "C:\Program Files (x86)\Steam\steamapps\common\DayZ\JM"
cmd.exe /c rmdir "P:\JM"

# RFFSHeli_Core
cmd.exe /c rmdir "P:\RFFSHeli_Core"

# HypeTrain Core
cmd.exe /c rmdir "P:\HypeTrain"

# Missions
cmd.exe /c rmdir "C:\Program Files (x86)\Steam\steamapps\common\DayZ\missions"

/mnt/c/Program\ Files\ \(x86\)/Steam/steamapps/common/DayZ\ Tools/Bin/WorkDrive/WorkDrive.exe /Dismount P
