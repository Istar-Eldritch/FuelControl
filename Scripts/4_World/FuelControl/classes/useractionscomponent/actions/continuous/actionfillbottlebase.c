modded class ActionFillBottleBase: ActionContinuousBase {

	bool pumpHasFuel = true;
	
	override string GetText(){
		if (pumpHasFuel){
			return super.GetText();
		}
		return "There is no fuel at this station";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		if (super.ActionCondition( player, target, item )) {
			FuelStation station = FuelStation.Cast(target.GetObject());
			if (station){
				GetFuelStationManager().SendRequestStation(station.GetPosition());
				// TODO: Find a better way to notify hte player the station has no fuel.
				pumpHasFuel = station.HasFuel();
				return true;
			}
			return true;
		}
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data )) {
			FuelStation station = FuelStation.Cast(action_data.m_Target.GetObject());
			if (station){
				return station.HasFuel();
			}
			return true;
		}
		return false;
	}
};

modded class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
	
	override void CreateActionComponent() {
		
		FuelControlSettings settings = GetFuelControlSettings();
		float targetRate = settings.liquid_transfer_rates.Get(m_ActionData.m_Target.GetObject().GetType());
		if (!targetRate) {
			targetRate = UAQuantityConsumed.FUEL;
		}
		float itemRate = settings.liquid_transfer_rates.Get(m_ActionData.m_MainItem.GetType());
		if (!itemRate) {
			itemRate = UAQuantityConsumed.FILL_LIQUID;
		}
		
		float transferRate;
		
		if (targetRate < itemRate) {
			transferRate = targetRate;
		} else {
			transferRate = itemRate;
		}
		
		m_liquid_type = ActionFillBottleBase.Cast( m_ActionData.m_Action ).GetLiquidType( m_ActionData.m_Player, m_ActionData.m_Target, m_ActionData.m_MainItem );
		m_ActionData.m_ActionComponent = new CAContinuousFill(transferRate, m_liquid_type);
	}
};