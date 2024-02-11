
#ifdef HypeTrain
    modded class HypeTrain_LocomotiveBase {

        float m_fuelDebt = 0;
        float m_Autonomy = 0;
		int m_FuelType = LIQUID_DIESEL;

        override bool StartEngine(bool forceStart = false, bool skipStartup = false)
        {
            ReloadConfigs();
            return super.StartEngine(forceStart, skipStartup);
            Print("Starting engine!");
        }

        override protected void UpdateFuel(float dt) {
			if (m_Autonomy == 0) {
				super.UpdateFuel(dt);
			} else {
	            if ((m_EngineState == HypeTrain_EngineState.OFF) || !HasLiquidQuantity())
	                return;
	
	            float rpm;
	            if (!rpm) {
	                rpm = m_GeneratorRpm / m_GeneratorRpmMax;
	            }
	            
	            if (rpm < 0.1) {
	                rpm = 0.1;
	            }
	            
	            float speed = Math.AbsFloat(m_Velocity); // speed in m/s
	            float ds = dt * speed; // distance traveled in dt in m
	            
	            // fuel consumed in ds
	            float consumedFuel = (ds * GetLiquidQuantityMax() / m_Autonomy / 1000 + m_fuelDebt) * (rpm + 0.5);
	
	            if (consumedFuel > 1e-3) {
	                AddLiquidQuantity(-consumedFuel);
	                m_fuelDebt = 0;
	            } else {
	                m_fuelDebt = consumedFuel;
	            }
	        
	            // Shut off engine when empty
	            if (!HasEngineFuel())
	                StopEngine();
			}
        }

        void ReloadConfigs() {
            FuelControlSettings settings = GetFuelControlSettings();
            auto type = GetType();
            auto vehicle_config = settings.vehicle_config.Get(type);
            if (vehicle_config) {
                m_Autonomy = vehicle_config.autonomy;
                m_FuelType = IE_FC_LiquidFromString(vehicle_config.fuel_type);
                if (m_FuelType == -1) {
                    CF_Log.Error("[FuelControl] Couldn't parse fuel type for " + type + " likely a misconfiguration");
                    m_FuelType = LIQUID_DIESEL;
                }
            } else {
                m_Autonomy = 0;
                m_FuelType = LIQUID_DIESEL;
            }
	    }
    }
#endif