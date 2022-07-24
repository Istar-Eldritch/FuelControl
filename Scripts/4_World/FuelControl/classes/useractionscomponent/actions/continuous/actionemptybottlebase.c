modded class ActionEmptyBottleBaseCB : ActionContinuousBaseCB
{	
	override void CreateActionComponent()
	{
		FuelControlSettings settings = GetFuelControlSettings();
		float itemRate = settings.liquid_transfer_rates.Get(m_ActionData.m_MainItem.GetType());
		if (!itemRate) {
			itemRate = UAQuantityConsumed.DRAIN_LIQUID;
		}
		
		m_ActionData.m_ActionComponent = new CAContinuousEmpty(itemRate);
	}
};