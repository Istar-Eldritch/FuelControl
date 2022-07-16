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
				Car car = Car.Cast(obj);
				Barrel_ColorBase barrel = Barrel_ColorBase.Cast(obj);
				if (car) {
					car.Fill( CarFluid.FUEL, (m_SpentQuantity / 1000) );
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

class ActionFillAtStationCB : ActionContinuousBaseCB {
	private const float TIME_TO_REPEAT = 0.5;

	override void CreateActionComponent() {
		m_ActionData.m_ActionComponent = new CAFillAtStation( UAQuantityConsumed.FUEL, TIME_TO_REPEAT );
	}
};

class ActionFillAtStation : ActionContinuousBase {
	
	vector refillPointPos;
	bool nearbyStation = false;
	bool pumpHasFuel = false;
	
	void ActionFillAtStation() {
		m_CallbackClass = ActionFillAtStationCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = true;
	}
	
	override void CreateConditionComponents() {
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
	
	override string GetText() {
		if (pumpHasFuel) {
			return "#refuel";
		} else {
			return "There is no fuel at this station";
		}
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	void CheckNearbyStations(vector pos) {
		autoptr auto objects = new array<Object>;
		autoptr auto proxycargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(pos, 5, objects, proxycargos);
		foreach(auto object : objects) {
			FuelStation station = FuelStation.Cast(object);
			if (station) {
				pumpHasFuel = station.HasFuel();
				nearbyStation = true;
				break;
			} else {
				pumpHasFuel = false;
				nearbyStation = false;
			}
		}
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		
		FuelControlSettings settings = GetFuelControlSettings();
		

		Car car = Car.Cast( target.GetObject() );
		Barrel_ColorBase barrel = Barrel_ColorBase.Cast(target.GetObject());
	
		if (!car && !barrel)
			return false;
		
		if (settings.pump_car_refueling && car && car.GetFluidFraction(CarFluid.FUEL) < 0.98) {
			array<string> selections = new array<string>;
			target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);
	
			CarScript carS = CarScript.Cast(car);
			
			if ( carS ) {
				for (int s = 0; s < selections.Count(); s++) {
					if ( selections[s] == carS.GetActionCompNameFuel() ) {
						refillPointPos = carS.GetRefillPointPosWS();
						float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
						float distanceFuel = carS.GetActionDistanceFuel() * carS.GetActionDistanceFuel();
						if(dist < distanceFuel) {
							GetFuelStationManager().SendRequestStation(refillPointPos);
							CheckNearbyStations(refillPointPos);
							return nearbyStation;		
						}
					}
				}
			}
		} else if (settings.pump_barrel_refueling && barrel && barrel.IsOpen() && Liquid.CanFillContainer(barrel, LIQUID_GASOLINE)) {
			refillPointPos = barrel.GetPosition();
			GetFuelStationManager().SendRequestStation(refillPointPos);
			CheckNearbyStations(refillPointPos);
			return nearbyStation;	
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data )) {
			CheckNearbyStations(refillPointPos);
			return pumpHasFuel;
		}
		return false;
	}
};