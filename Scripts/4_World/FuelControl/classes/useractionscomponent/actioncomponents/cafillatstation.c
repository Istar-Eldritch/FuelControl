class CAFillAtStation : CAContinuousBase {
	
	protected vector 				location;
	protected PlayerBase 			m_Player;
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_EmptySpace; //basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_AdjustedQuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;
	
	void CAFillAtStation( float quantity_used_per_second, float time_to_progress ) {
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}
	
	override void Setup(ActionData action_data) {

		m_Player = action_data.m_Player;
		
		m_TimeElpased = 0;
		m_SpentQuantity = 0;

		
		if ( !m_SpentUnits ) {
			m_SpentUnits = new Param1<float>( 0 );
		} else {
			m_SpentUnits.param1 = 0;
		}
		
		auto obj = action_data.m_Target.GetObject();
		location = obj.GetPosition();
		Car car = Car.Cast(obj);
		Barrel_ColorBase barrel = Barrel_ColorBase.Cast(obj);
		
		float fuelCapacity;
		float currentFuel;
		if (car) {
			fuelCapacity = car.GetFluidCapacity( CarFluid.FUEL );
			currentFuel = car.GetFluidFraction( CarFluid.FUEL );
			currentFuel = currentFuel * fuelCapacity;
	
			m_EmptySpace = (fuelCapacity - currentFuel) * 1000;
		} else if (barrel) {
			fuelCapacity = barrel.GetQuantityMax();
			currentFuel = barrel.GetQuantity();
			m_EmptySpace = (fuelCapacity - currentFuel);
		}
		
		m_ItemQuantity = m_EmptySpace;

	}
	
		//---------------------------------------------------------------------------
	override int Execute( ActionData action_data  ) {
		
		if ( !action_data.m_Player ) {
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 ) {
			return UA_FINISHED;
		}
		else {
			if ( m_SpentQuantity_total < m_ItemQuantity )
			{
				m_AdjustedQuantityUsedPerSecond = action_data.m_Player.GetSoftSkillsManager().SubtractSpecialtyBonus( m_QuantityUsedPerSecond, m_Action.GetSpecialtyWeight(), true);
				m_SpentQuantity += m_AdjustedQuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();
				
				if ( m_TimeElpased >= m_DefaultTimeStep )
				{
					CalcAndSetQuantity( action_data );
					m_TimeElpased = 0;
					//Setup(action_data);	//reset data after repeat
				}
				
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity( action_data );
				OnCompletePogress(action_data);
				return UA_FINISHED;
			}
		}
	}
	
	//---------------------------------------------------------------------------
	override int Cancel( ActionData action_data ) {
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity( action_data );
		return UA_INTERRUPT;
	}	
	
	//---------------------------------------------------------------------------
	override float GetProgress() {
		if ( m_ItemQuantity <= 0 )
			return 1;

		return -(m_SpentQuantity_total / m_ItemQuantity);
	}
	
	void CalcAndSetQuantity(ActionData action_data) {
		m_SpentQuantity_total += m_SpentQuantity;
	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		
		
		if ( GetGame().IsServer() ) {
			
			
			autoptr auto objects = new array<Object>;
			autoptr auto proxycargos = new array<CargoBase>;
			GetGame().GetObjectsAtPosition(location, 5, objects, proxycargos);
			FuelStation station;
			foreach(auto object : objects) {
				station = FuelStation.Cast(object);
				if (station != null)
					break;
			}
			
			if(station) {
				Object obj = action_data.m_Target.GetObject();
				CarScript car = CarScript.Cast(obj);
				Barrel_ColorBase barrel = Barrel_ColorBase.Cast(obj);
				if (car) {
					car.AddFuel(m_SpentQuantity / 1000 );
				} else if(barrel) {
					Liquid.FillContainer(barrel, LIQUID_GASOLINE, m_SpentQuantity);
				}

				station.RemoveFuel(m_SpentQuantity);
				// This is likely to be very ineficient. There should be a better way of doing this.
				GetFuelStationManager().Save();
			}
		}
		m_SpentQuantity = 0;
	}
};
