class ActionMeasureFuel: ActionSingleUseBase {
	
	float fuel;
	bool showFuel;

	void ActionAttach() {
	}
	
	override string GetText() {
		if (showFuel) {
			if (fuel == -1) {
				return "You can't reach the bottom";
			}
			return "" + fuel + "L";
		}
		return "Measure fuel level";
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
			array<string> selections = new array<string>;
			target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);
            for (int s = 0; s < selections.Count(); s++) {
                if ( selections[s] == car.GetActionCompNameFuel() ) {
                    vector refillPointPos = car.GetRefillPointPosWS();
                    float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
                    float distanceFuel = car.GetActionDistanceFuel() * car.GetActionDistanceFuel();
                    if (dist <= distanceFuel) {
                        return true;
                    }
                }
            }
        }
		return false;
	}

    override void OnExecute( ActionData action_data ) {

        auto target = action_data.m_Target;

        FuelStation station = FuelStation.Cast( target.GetObject() );
		
		if ( station ) {
			fuel = station.GetFuel();
			showFuel = true;
		}

		CarScript car = CarScript.Cast( target.GetObject() );
        if (car) {
            float fuelCapacity = car.GetFluidCapacity( CarFluid.FUEL );
            float currentFuel = car.GetFluidFraction( CarFluid.FUEL );
            currentFuel = currentFuel * fuelCapacity;
            fuel = currentFuel;
            showFuel = true;
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ResetText, 3000);

	}

    void ResetText() {
		showFuel = false;
    }
}