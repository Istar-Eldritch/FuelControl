class IEActionTurnOn: ActionInteractBase
{	
	void IEActionTurnOn()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#switch_on";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		EntityAI pg = EntityAI.Cast(target.GetObject());

		if (pg)
		{
			if (pg.HasEnergyManager() && pg.GetCompEM().CanSwitchOn() && pg.GetCompEM().CanWork())
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Object targetObject = action_data.m_Target.GetObject();
		if (targetObject)
		{
			EntityAI target_IB = EntityAI.Cast(targetObject);
			target_IB.GetCompEM().SwitchOn();
			target_IB.GetCompEM().InteractBranch(target_IB);
		}
	}
};