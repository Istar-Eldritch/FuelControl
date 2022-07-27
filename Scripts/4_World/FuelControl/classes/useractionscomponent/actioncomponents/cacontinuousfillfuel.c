modded class CAContinuousFillFuel: CAContinuousBase {
	override void CalcAndSetQuantity( ActionData action_data )
	{

		m_SpentQuantity_total += m_SpentQuantity;
	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		
		
		if ( GetGame().IsServer() )
		{
			CarScript car = CarScript.Cast(action_data.m_Target.GetObject());	
			action_data.m_MainItem.AddQuantity( -m_SpentQuantity );
			car.AddFuel( m_SpentQuantity / 1000 );
		}
		
		m_SpentQuantity = 0;
	}
}