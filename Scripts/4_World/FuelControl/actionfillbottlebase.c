
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

modded class CAContinuousFill : CAContinuousBase
{
	override void CalcAndSetQuantity( ActionData action_data )
	{
		FuelStation station = FuelStation.Cast(action_data.m_Target.GetObject());
		ItemBase theBottle = ItemBase.Cast(action_data.m_MainItem);
		if (station && theBottle && GetGame().IsServer()){
			station.RemoveFuel(m_SpentQuantity);
			// This is likely to be very ineficient. There should be a better way of doing this.
			GetFuelStationManager().Save();
		}
		super.CalcAndSetQuantity( action_data );
	}
	
 };