modded class ActionFillFuelCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.5;

	override void CreateActionComponent()
	{
		FuelControlSettings settings = GetFuelControlSettings();
		float targetRate = settings.liquid_transfer_rates.Get(m_ActionData.m_Target.GetObject().GetType());
		if (!targetRate) {
			targetRate = UAQuantityConsumed.FUEL;
		}
		float itemRate = settings.liquid_transfer_rates.Get(m_ActionData.m_MainItem.GetType());
		if (!itemRate) {
			itemRate = UAQuantityConsumed.DRAIN_LIQUID;
		}
		
		float transferRate;
		
		if (targetRate < itemRate) {
			transferRate = targetRate;
		} else {
			transferRate = itemRate;
		}

		m_ActionData.m_ActionComponent = new CAContinuousFillFuel(transferRate, TIME_TO_REPEAT);
		
	}
};

modded class ActionFillFuel: ActionContinuousBase
{
	
	int GetFuelType() {
		return -1;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !IsTransport(target) )
			return false;

		if ( item.GetQuantity() <= 0 )
			return false;

		if ( item.GetLiquidType() != GetFuelType())
			return false;

		CarScript car = CarScript.Cast( target.GetObject() );
		if ( !car)
			return false;
	
		if (car.GetFuelType() != GetFuelType())
			return false;
		
		if (car.IsDamageDestroyed())
		{
			return false;
		}
		
		if ( car.GetFluidFraction( CarFluid.FUEL ) >= 0.98 )
			return false;

		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		CarScript carS = CarScript.Cast(car);
		
		if ( carS )
		{
			for (int s = 0; s < selections.Count(); s++)
			{
				if ( selections[s] == carS.GetActionCompNameFuel() )
				{
					float dist = vector.DistanceSq( carS.GetRefillPointPosWS(), player.GetPosition() );

					if ( dist < carS.GetActionDistanceFuel() * carS.GetActionDistanceFuel() )
						return true;
				}
			}
		}

		return false;
	}
};

class ActionFillGasolineFuel: ActionFillFuel {
	
	override string GetText() {
		return "Refuel Gasoline";
	}
	
	override int GetFuelType() {
		return LIQUID_GASOLINE;
	}
}

class ActionFillDieselFuel: ActionFillFuel {
	
	override int GetFuelType() {
		return LIQUID_DIESEL;
	}
	
	override string GetText() {
		return "Refuel Diesel";
	}
}

class ActionFillAvGasFuel: ActionFillFuel {
	override string GetText() {
		return "Refuel Aviation Fuel";
	}

	override int GetFuelType() {
		return IE_FC_LIQUID_AVGAS;
	}
}