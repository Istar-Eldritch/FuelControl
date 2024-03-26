class ActionMeasureFuel: ActionSingleUseBase {
	
	override string GetText() {
		string fuelName = IE_FC_UIStringForLiquid(GetFuelType());
		fuelName.ToLower();
		string measureLoc = Widget.TranslateString( "#STR_IEFC_MEASURE_FUELTYPE" );
		measureLoc.Replace("{fuel_type}", fuelName);
		return measureLoc;
	}
	
	int GetFuelType() {
		return -1;
	}
	
	string GetFuelName() {
		string fuelName = IE_FC_UIStringForLiquid(GetFuelType());
		fuelName.ToLower();
		return fuelName;
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		FuelStation station = FuelStation.Cast( target.GetObject() );

        auto config = GetFuelControlSettings();
        if (!config.settings.measure_fuel_with_stick) {
            return false;
        }
		
		if ( station && item ) {
			return true;
		}

        CarScript car = CarScript.Cast( target.GetObject() );

        if (car && item) {
			if (car.GetFuelType() != GetFuelType()) {
				return false;
			}
			array<string> selections = new array<string>;
			target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);
            for (int s = 0; s < selections.Count(); s++) {
                if ( selections[s] == car.GetActionCompNameFuel() ) {
                    vector refillPointPos = car.GetRefillPointPosWS();
                    float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
                    float distanceFuel = car.GetActionDistanceFuel() * car.GetActionDistanceFuel();
                    if (dist <= distanceFuel) {
						if (GetGame().IsServer()) {
							car.Synchronize();
						}
                        return true;
                    }
                }
            }
        }

		#ifdef HypeTrain
			auto trainTarget = HypeTrain_PartBase.Cast(target.GetObject());

			if (trainTarget) {
				auto cmpName = target.GetObject().GetActionComponentName(target.GetComponentIndex());
				if(cmpName.Contains("pour") && trainTarget.GetLiquidType() == GetFuelType())
					return true;
			}
		#endif

		return false;
	}

    override void OnExecute( ActionData action_data ) {
		if (GetGame().IsClient()) {
	        auto target = action_data.m_Target;
	
	        FuelStation station = FuelStation.Cast( target.GetObject() );
			
			float fuel;
			if ( station ) {
				fuel = station.GetFuel(GetFuelType());
			}
	
			CarScript car = CarScript.Cast( target.GetObject() );
	        if (car) {
	            fuel = car.GetFuelAmount();
	        }

			#ifdef HypeTrain
				auto trainTarget = HypeTrain_PartBase.Cast(target.GetObject());
				if (trainTarget) {
					fuel = trainTarget.GetLiquidQuantity() / 1000;
				}
			#endif

			string formatText = "{fuel_amount} " + GetFuelName();
			if (fuel == -1) {
				string infiniteLoc = Widget.TranslateString( "#STR_IEFC_INFINITE" );
				formatText.Replace("{fuel_amount}", infiniteLoc);
			} else {
				int fuelInt = fuel * 100;
				fuel = fuelInt / 100;
				formatText.Replace("{fuel_amount}", "" + fuel + "l of");
			}
			GetGame().Chat(formatText, "colorImportant");
		}
	}
}

class ActionMeasureDiesel: ActionMeasureFuel {
	
	override int GetFuelType() {
		return LIQUID_DIESEL;
	}
}

class ActionMeasureGasoline: ActionMeasureFuel {

	override int GetFuelType() {
		return LIQUID_GASOLINE;
	}
}

class ActionMeasureAvGas: ActionMeasureFuel {
	override int GetFuelType() {
		return IE_FC_LIQUID_AVGAS;
	}
}