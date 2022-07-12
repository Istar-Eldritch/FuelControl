
modded class ActionFillBottleBase: ActionContinuousBase {

	// TODO GET TEXT
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		if (super.ActionCondition( player, target, item )) {
			FuelStation station = FuelStation.Cast(target.GetObject());
			if (station){
				return station.HasFuel();
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