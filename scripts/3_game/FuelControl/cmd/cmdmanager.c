class CmdHandler {
    bool CanHandle(ref CmdArgs cmd) {}
    void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {}
}

class CmdManager {

    ref array<ref CmdHandler> handlers = new ref array<ref CmdHandler>;

    bool CanHandle(ref CmdArgs cmd) {
        foreach(auto handler: handlers) {
            if (handler.CanHandle(cmd)) {
                return true;
            }
        }
        return false;
    }

    void RegisterHandler(ref CmdHandler handler) {
        handlers.Insert(handler);
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

            Param1<CmdArgs> data;
		    if (ctx.Read(data)) {
			    ref CmdArgs cmd = data.param1;
                foreach(auto handler: handlers) {
                    if (handler.CanHandle(cmd)) {
                        handler.HandleCmd(cmd, sender);
                        break;
                    }
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

}

static ref CmdManager g_CmdManager;
static ref CmdManager GetCmdManager() {
    if (!g_CmdManager) {
        g_CmdManager = new ref CmdManager();
    }
    return g_CmdManager;
}