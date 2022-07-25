modded class ChatInputMenu
{
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (!finished) return false;
		
		string text = m_edit_box.GetText();
		if (text != "" && (text.Substring(0,1)) == "#") {
			auto cmdLength = text.Length();
			auto stripped = text.Substring(1, cmdLength - 1);
            auto cmd = CmdParser.Parse(stripped);
			if (GetCmdManager().CanHandle(cmd)) {
				GetRPCManager().SendRPC("FuelControl", "HandleChatCommand", new Param1<ref CmdArgs>(cmd), true);
				m_close_timer.Run(0.1, this, "Close"); //Exit out as we don't want to send commands in chat for others to see.
				return true;
			}
		}
		
		return super.OnChange(w, x, y, finished);
	}
};