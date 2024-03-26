class IEActionTurnOff: ActionInteractBase
{	
	void IEActionTurnOff()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#switch_off";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		EntityAI target_IB = EntityAI.Cast( targetObject );
		
		if ( target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOff() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnExecuteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if ( targetObject )
		{
			EntityAI target_IB = EntityAI.Cast( targetObject );
			target_IB.GetCompEM().SwitchOff();
			target_IB.GetCompEM().InteractBranch(target_IB);
		}
	}
};