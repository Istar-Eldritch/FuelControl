class CASiphon : CAContinuousFill {
	
	override void Setup(ActionData action_data) {
		super.Setup(action_data);
	}
	
	override void CalcAndSetQuantity(ActionData action_data) {
		m_SpentQuantity_total += m_SpentQuantity;

		if ( m_SpentUnits ) {
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		
		if (GetGame().IsServer()) {
			CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
			car.RemoveFuel(m_SpentQuantity / 1000);
			Liquid.FillContainerEnviro(action_data.m_MainItem, m_liquid_type, m_SpentQuantity, false);
		}
		
		m_SpentQuantity = 0;

	}
};