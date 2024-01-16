class ActionFillAtStationCB : ActionContinuousBaseCB {
	private const float TIME_TO_REPEAT = 0.25;

	override void CreateActionComponent() {
		FuelControlSettings settings = GetFuelControlSettings();
		float containerRate = settings.liquid_transfer_rates.Get(m_ActionData.m_Target.GetObject().GetType());
		if (!containerRate) {
			containerRate = UAQuantityConsumed.FUEL;
		}
		
		float stationRate = settings.liquid_transfer_rates.Get("Land_FuelStation_Feed");
		
		if (!stationRate) {
			stationRate = UAQuantityConsumed.FUEL;
		}
		
		float transferRate;
		
		if (stationRate <= containerRate) {
			transferRate = stationRate;
		} else {
			transferRate = containerRate;
		}
		
		
		m_ActionData.m_ActionComponent = new CAFillAtStation( transferRate, TIME_TO_REPEAT );
	}
};


class ActionFillAtStation : ActionContinuousBase {
	
	vector refillPointPos;
	bool nearbyStation = false;
	bool pumpHasFuel = false;
	bool pumpHasEnergy = false;
	
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
		if (pumpHasFuel && pumpHasEnergy) {
			return "#refuel";
		} else if (!pumpHasEnergy) {
			return "Fuel pumps require energy to run";
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
				
				pumpHasFuel = !station.IsRuined() && station.HasFuel();
				pumpHasEnergy = station.HasEnergy();
				nearbyStation = !station.IsRuined();
				break;
			} else {
				pumpHasFuel = false;
				nearbyStation = false;
			}
		}
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		
		FuelControlSettings config = GetFuelControlSettings();
		

		Car car = Car.Cast( target.GetObject() );
		Barrel_ColorBase barrel = Barrel_ColorBase.Cast(target.GetObject());
	
		if (!car && !barrel)
			return false;
		
		if (config.settings.pump_car_refueling && car && car.GetFluidFraction(CarFluid.FUEL) < 0.98) {
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
							FuelStationManager manager = GetFuelStationManager();
							manager.FindStationForPump(refillPointPos);
							CheckNearbyStations(refillPointPos);
							return nearbyStation;		
						}
					}
				}
			}
		} else if (config.settings.pump_barrel_refueling && barrel && barrel.IsOpen() && Liquid.CanFillContainer(barrel, LIQUID_GASOLINE)) {
			refillPointPos = barrel.GetPosition();
			CheckNearbyStations(refillPointPos);
			return nearbyStation;	
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data )) {
			CheckNearbyStations(refillPointPos);
			return pumpHasFuel && pumpHasEnergy;
		}
		return false;
	}
};