

modded class CAContinuousFill : CAContinuousBase
{	
	protected FuelStation m_station;

	override void Setup( ActionData action_data )
	{
		auto old = m_QuantityFilledPerSecond;
		super.Setup(action_data);
		m_QuantityFilledPerSecond = old;
		m_AdjustedQuantityFilledPerSecond = action_data.m_Player.GetSoftSkillsManager().AddSpecialtyBonus( old, m_Action.GetSpecialtyWeight(), true );
		m_station = FuelStation.Cast(action_data.m_Target.GetObject());
	}
	
	override void CalcAndSetQuantity( ActionData action_data )
	{
		ItemBase theBottle = ItemBase.Cast(action_data.m_MainItem);
		if (m_station && theBottle && GetGame().IsServer()){
			m_station.RemoveFuel(m_liquid_type, m_SpentQuantity / 1000);
			// This is likely to be very ineficient. There should be a better way of doing this.
			if (GetGame().IsServer()) {
				GetFuelStationManager().SaveState();
			}
		}
		
		super.CalcAndSetQuantity( action_data );
	}
	
 };