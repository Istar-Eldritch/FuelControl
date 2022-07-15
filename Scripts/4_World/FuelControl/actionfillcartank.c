class CAFillCarTank : CAContinuousFillFuel {
	
	override void Setup(ActionData action_data) {

		m_Player = action_data.m_Player;
		
		Car car = Car.Cast(action_data.m_Target.GetObject());
		
		m_TimeElpased = 0;
		m_SpentQuantity = 0;
		
		if ( !m_SpentUnits )
		{
			m_SpentUnits = new Param1<float>( 0 );
		}
		else
		{
			m_SpentUnits.param1 = 0;
		}

		float fuelCapacity = car.GetFluidCapacity( CarFluid.FUEL );
		float currentFuel = car.GetFluidFraction( CarFluid.FUEL );
		currentFuel = currentFuel * fuelCapacity;

		m_EmptySpace = (fuelCapacity - currentFuel) * 1000;
		m_ItemQuantity = m_EmptySpace;
	}
	
	override void CalcAndSetQuantity(ActionData action_data) {
		m_SpentQuantity_total += m_SpentQuantity;
	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		
		
		if ( GetGame().IsServer() )
		{
			
			Car car = Car.Cast(action_data.m_Target.GetObject());
			vector carPos = car.GetPosition();
			
			autoptr auto objects = new array<Object>;
			autoptr auto proxycargos = new array<CargoBase>;
			GetGame().GetObjectsAtPosition(carPos, 5, objects, proxycargos);
			FuelStation station;
			foreach(auto object : objects) {
				station = FuelStation.Cast(object);
				if (station != null)
					break;
			}
			
			if(station) {
				station.RemoveFuel(m_SpentQuantity);
				car.Fill( CarFluid.FUEL, (m_SpentQuantity / 1000) );

				// This is likely to be very ineficient. There should be a better way of doing this.
				GetFuelStationManager().Save();
			}
			
		}

		m_SpentQuantity = 0;
	}
};

class ActionFillCarTankCB : ActionContinuousBaseCB {
	private const float TIME_TO_REPEAT = 0.5;

	override void CreateActionComponent() {
		m_ActionData.m_ActionComponent = new CAFillCarTank( UAQuantityConsumed.FUEL, TIME_TO_REPEAT );
	}
};

class ActionFillCarTank : ActionContinuousBase {
	
	vector refillPointPos;
	bool nearbyStation = false;
	bool pumpHasFuel = false;
	
	void ActionFillCarTank() {
		m_CallbackClass = ActionFillCarTankCB;
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
		
		if(!settings.pump_refueling)
			return false;

		Car car = Car.Cast( target.GetObject() );
		if (!car || car.GetFluidFraction(CarFluid.FUEL) >= 0.98)
			return false;
		
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

modded class ActionConstructor {

	override void RegisterActions(TTypenameArray actions) {
		super.RegisterActions(actions);
		actions.Insert(ActionFillCarTank);
		actions.Insert(ActionSiphon);
	}
};

modded class CarScript {
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillCarTank);
		AddAction(ActionSiphon);
	}
};