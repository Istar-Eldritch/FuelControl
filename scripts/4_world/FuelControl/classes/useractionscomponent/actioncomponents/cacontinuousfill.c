

modded class CAContinuousFill : CAContinuousBase
{
	
	override void Setup( ActionData action_data )
	{
		auto old = m_QuantityFilledPerSecond;
		super.Setup(action_data);
		m_QuantityFilledPerSecond = old;
		m_AdjustedQuantityFilledPerSecond = action_data.m_Player.GetSoftSkillsManager().AddSpecialtyBonus( old, m_Action.GetSpecialtyWeight(), true );
	}

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