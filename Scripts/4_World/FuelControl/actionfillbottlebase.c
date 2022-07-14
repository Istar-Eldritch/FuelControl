
modded class ActionFillBottleBase: ActionContinuousBase {

	bool pumpHasFuel = true;
	
	override string GetText(){
		if (pumpHasFuel){
			return super.GetText();
		}
		return "There is no fuel on this station";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		if (super.ActionCondition( player, target, item )) {
			FuelStation station = FuelStation.Cast(target.GetObject());
			if (station){
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
			// Notify all the clients that this station fuel was updated.
			// XXX: Not sure if this is necesary and I'm also not sure how much of a performance impact this will have with lots of players.
			// It may be better to trigger a request from the client side when a player gets close to a fuel station.
			GetRPCManager().SendRPC("FuelControl", "UpdateStation", new Param1<FuelStationGroup>(GetFuelStationManager().FindStationForPump(station.GetPosition())), true);
			
			// This is likely to be very ineficient. There should be a better way of doing this.
			GetFuelStationManager().Save();
		}
		super.CalcAndSetQuantity( action_data );
	}
	
 };