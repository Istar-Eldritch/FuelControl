modded class ActionPlugTargetIntoThis: ActionSingleUseBase
{
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{		
		EntityAI targetIB = EntityAI.Cast(target.GetObject());
		
		//Prevent plugging to items in inventory
		if (targetIB && targetIB.GetHierarchyRoot() == targetIB && item && targetIB.HasEnergyManager() && item.HasEnergyManager())
		{
			if (!targetIB.GetCompEM().IsPlugged() && item.GetCompEM().CanReceivePlugFrom(targetIB))
			{
				//hotfix, re-work entire action!
				return targetIB.IsElectricAppliance();
			}
			
			// Special case for vehicle batteries
			if (item.IsInherited(VehicleBattery))
			{
				MetalWire metalWire = MetalWire.Cast(item.GetCompEM().GetPluggedDevice());
				
				if (metalWire && metalWire.GetCompEM().CanReceivePlugFrom(targetIB))
				{
					return true; // We can power the action_data.m_Target from vehicle battery because it has metal wire attached.
				}
			}
		}
		
		return false;
	}
	

	override void Process(ActionData action_data)
	{
		EntityAI targetIB = EntityAI.Cast(action_data.m_Target.GetObject());

		if (action_data.m_MainItem.IsInherited(VehicleBattery))
		{
			// Car/truck batteries can have a metal wire attached through which they can power common electric appliances
			MetalWire metalWire = MetalWire.Cast(action_data.m_MainItem.GetCompEM().GetPluggedDevice());
			if (metalWire)
				targetIB.GetCompEM().PlugThisInto(metalWire);
		}
		else
		{
			// Everything else in general
			targetIB.GetCompEM().PlugThisInto(action_data.m_MainItem);
		}
		
		Spotlight spotlight;
		if (Spotlight.CastTo(spotlight, targetIB))
			spotlight.Unfold();
		
		GameInventory inventory = targetIB.GetInventory();
		if (inventory) {
			inventory.TakeEntityAsAttachment(InventoryMode.LOCAL, action_data.m_MainItem);
		}
		action_data.m_Player.ServerDropEntity(action_data.m_MainItem);
	}
}
