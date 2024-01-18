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
	
	protected FuelStation station;
	protected CarScript vehicle;

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
		auto hasFuel = station.HasFuel();
		auto hasEnergy = station.HasEnergy();
		if (hasFuel && hasEnergy) {
			return "#refuel";
		} else if (!hasFuel) {
			return "There is no fuel at this station";
		} else {
			return "Fuel pumps require energy to run";
		}
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	FuelStation CheckNearbyStations(vector pos) {
		autoptr auto objects = new array<Object>;
		autoptr auto proxycargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(pos, 5, objects, proxycargos);
		FuelStation f;
		float closest = 10;
		foreach(auto object : objects) {
			FuelStation s = FuelStation.Cast(object);
			if (s) {
				float d = vector.DistanceSq(pos, s.GetWorldPosition() );
				if (d < closest) {
					f  = s;
					closest = d;
				}
			}
		}
		
		return f;
	}
	
	override void OnStartAnimationLoop(ActionData action_data) {
		super.OnStartAnimationLoop(action_data);
		if (GetGame().IsServer() && station && station.HasFuel() && station.HasEnergy()) {
			station.SetWorking(true);
			if (vehicle) {
				vehicle.SetRefueling(true);
			}
		}
	}
	
	override void OnEndAnimationLoop(ActionData action_data) {
		super.OnEndAnimationLoop(action_data);
		if (GetGame().IsServer()) {
			if (station) {
				station.SetWorking(false);
			}
			if (vehicle) {
				vehicle.SetRefueling(false);
			}
		}
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		
		FuelControlSettings config = GetFuelControlSettings();
		

		Car car = Car.Cast( target.GetObject() );
		Barrel_ColorBase barrel = Barrel_ColorBase.Cast(target.GetObject());
	
		if (!car && !barrel)
			return false;
		
		vector refillPointPos;

		if (config.settings.pump_car_refueling && car && car.GetFluidFraction(CarFluid.FUEL) < 0.98) {
			array<string> selections = new array<string>;
			target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

			CarScript carS = CarScript.Cast(car);
			vehicle = carS;
			
			if ( carS ) {
				for (int s = 0; s < selections.Count(); s++) {
					if ( selections[s] == carS.GetActionCompNameFuel() ) {
						refillPointPos = carS.GetRefillPointPosWS();
						float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
						float distanceFuel = carS.GetActionDistanceFuel() * carS.GetActionDistanceFuel();
						if(dist < distanceFuel) {
							if (station == null) {
								station = CheckNearbyStations(refillPointPos);
							}
							return station != null;		
						}
					}
				}
			}
		} else if (config.settings.pump_barrel_refueling && barrel && barrel.IsOpen() && Liquid.CanFillContainer(barrel, LIQUID_GASOLINE)) {
			refillPointPos = barrel.GetPosition();
			if (station == null) {
				station = CheckNearbyStations(refillPointPos);
			}
			return station != null;	
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data ) && station) {
			return station.HasFuel() && station.HasEnergy();
		}
		return false;
	}
};