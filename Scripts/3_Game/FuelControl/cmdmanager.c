class ChatCmd {
    string name;
	ref array<string> args;
    void ChatCmd(ref array<string> tokens) {
        name = tokens.Get(0);
		tokens.Remove(0);
		args = tokens;
    }

    static ref ChatCmd FromString(string text) {
		auto tokens = ParseCMD(text);
        return new ref ChatCmd(tokens);
    }
}

ref Param2<string, string> ParseQuotedString(string cmd) {
    if (cmd.Length() > 0 && cmd[0] == "\"") {
        int i;
        string acc = "";
        for (i = 1; i < cmd.Length(); i++) {
            if (cmd[i] == "\"") {
                return new ref Param2<string, string>(acc, cmd.Substring(i + 1, cmd.Length()));
            }
            acc = acc + cmd[i];
        }
    }

    return null;
}

ref Param2<string, string> ParseString(string cmd) {
    if (cmd.Length() > 0) {
        int i;
        string acc = "";
        for (i = 0; i < cmd.Length(); i++) {
            if (cmd[i] == " ") {
                break;
            }
            acc = acc + cmd[i];
        }
		return new ref Param2<string, string>(acc, cmd.Substring(acc.Length(), cmd.Length()));
    }

    return null;
}

ref Param2<string, string> ParseSpace(string cmd) {
    if (cmd.Length() > 0) {
        int i;
        string acc = "";
        for (i = 0; i < cmd.Length(); i++) {
            if (cmd[i] != " ") {
                return new ref Param2<string, string>(acc, cmd.Substring(i, cmd.Length()));
            }
            acc = acc + cmd[i];
        }
    }

    return null;
}

ref array<string> ParseCMD(string cmd) {
    ref array<string> tokens = new ref array<string>;
    string processing = cmd;
    while (processing.Length() > 0) {
        ref Param2<string, string> parseResult;
        parseResult = ParseQuotedString(processing);
        if(parseResult) {
            tokens.Insert(parseResult.param1);
            processing = parseResult.param2;
        }

        parseResult = ParseString(processing);
        if(parseResult) {
            tokens.Insert(parseResult.param1);
            processing = parseResult.param2;
        }

        parseResult = ParseSpace(processing);
        if(parseResult) {
            processing = parseResult.param2;
        }

    }

    return tokens;
}

class CmdManager {

    string validCmds[4] = {
        "fc_addstation",
        "fc_setfuel",
        "fc_getstations",
        "fc_getfuel"
    };

    bool CanHandle(ref ChatCmd cmd) {
        int i;
        for (i = 0; i < 4; i++) {
            if (cmd.name == validCmds[i]) {
				return true;
			}
        }
        return false;
    }

    bool HasPermission(ref PlayerIdentity sender) {
        foreach(auto adminId: GetFuelControlSettings().settings.administrators) {
            if (adminId == sender.GetPlainId()) {
                return true;
            }
        }
        return false;
    }

    void HandleChatCommand(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
		if (GetGame().IsServer()) {
            if (!HasPermission(sender)) {
                Print("[FuelControl] Player " + sender.GetPlainId() + " tried to perform a command but is not in the administrators list");
				auto msg = new Param2<string, string>("You don't have permission to perform this action. This incident has been logged.", "colorImportant");
                GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", msg, true, sender);
                return;
            }

            Param1<ChatCmd> data;
		    if (ctx.Read(data)) {
			    ref ChatCmd cmd = data.param1;
                if (cmd.name == "fc_getfuel") {
                    GetFuel(cmd, sender);
                } else if (cmd.name == "fc_getstations") {
					GetStations(cmd, sender);
				}
                return;
            }
		}
	}

    void HandleChatMessage(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
		if (GetGame().IsClient()) {
            Param2<string, string> data;
		    if (ctx.Read(data)) {
			    string text = data.param1;
                string style = data.param2;
				// Valid styles
				// colorStatusChannel
				// colorAction
				// colorFriendly
				// colorImportant
                GetGame().Chat(text, style);
                return;
            }
		}
	}

    void GetFuel(ref ChatCmd cmd, ref PlayerIdentity sender) {
		
		auto stationName = cmd.args.Get(0);

        ref FuelStationGroup station;

        if (stationName) {
            station = GetFuelStationManager().FindStationByName(stationName);
        } else {
            array<Man> players = new array<Man>;
            GetGame().GetWorld().GetPlayerList(players);
            foreach (auto player: players) {
                if (player.GetIdentity().GetId() == sender.GetId()) {
                    auto playerLocation = player.GetPosition();
                    station = GetFuelStationManager().FindStationForPump(playerLocation);
                }
            }
        }

        string text;
        if (station) {
			Print("[FuelControl] Executing GetFuel chat command" + station.name);
            string amount;
            if (station.fuelAmount >= 0) {
                auto scaledAmount = station.fuelAmount / 1000;
                amount = "" + scaledAmount + "L";
            } else {
                amount = "Infinite";
            }
            text = "Fuel available at " + station.name + ": " + amount;
        } else {
            text = "Could not find the fuel station";
        }
        auto parameter = new Param2<string, string>(text, "colorStatusChannel");
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
    }
	
	void GetStations(ref ChatCmd cmd, ref PlayerIdentity sender) {
		
		foreach(auto station: GetFuelStationManager().stations) {
			string amount;
			if (station.fuelAmount >= 0) {
                auto scaledAmount = station.fuelAmount / 1000;
                amount = "" + scaledAmount + "L";
            } else {
                amount = "Infinite";
            }
			string text = station.name + ": " + amount;
			auto parameter = new Param2<string, string>(text, "colorStatusChannel");
			GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
		}
    }
}

static ref CmdManager g_CmdManager;
static ref CmdManager GetCmdManager() {
    if (!g_CmdManager) {
        g_CmdManager = new ref CmdManager();
    }
    return g_CmdManager;
}