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