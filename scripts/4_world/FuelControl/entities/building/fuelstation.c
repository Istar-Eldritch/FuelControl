modded class FuelStation {
	
	protected FuelStationGroup group = null;
	protected bool m_ruined;
	protected bool m_working;
	
	void FuelStation() {
		CreateComponent(COMP_TYPE_ENERGY_MANAGER);
		RegisterNetSyncVariableBool("m_EM.m_IsSwichedOn");
		RegisterNetSyncVariableBool("m_EM.m_CanWork");
		RegisterNetSyncVariableBool("m_EM.m_IsPlugged");
		RegisterNetSyncVariableInt("m_EM.m_EnergySourceNetworkIDLow");
		RegisterNetSyncVariableInt("m_EM.m_EnergySourceNetworkIDHigh");
		RegisterNetSyncVariableFloat("m_EM.m_Energy");
		RegisterNetSyncVariableBool("m_ruined");
		RegisterNetSyncVariableBool("m_working");
		Print("Initialized station");
	}

	override void EEKilled(Object killer) {
		super.EEKilled(killer);
		auto config = GetFuelControlSettings();
		if(config.settings.pumps_get_ruined) {
			m_ruined = true;
			SetSynchDirty();
		}
	}

	override bool IsElectricAppliance()
	{
		return true;
	}
	
	override void OnInitEnergy()
	{
		super.OnInitEnergy();
		GetCompEM().SetEnergyUsage(1);
		GetCompEM().SetUpdateInterval(1);
		GetCompEM().SwitchOn();
		SetSynchDirty();
	}
	
	override void OnWorkStart()
	{
		super.OnWorkStart();
		m_working = true;
		SetSynchDirty();
	}
	
	override void OnWorkStop()
	{
		super.OnWorkStop();
		m_working = false;
		SetSynchDirty();
	}
	
	override void OnVariablesSynchronized() {
		super.OnVariablesSynchronized();
	}
	
	override void OnWork(float consumed_energy) {
		super.OnWork(consumed_energy);
		Print("Consumed " + consumed_energy);
	}
	
	float GetFuel() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}
		
		if(group) {
			return group.GetFuel();
		}
		
		auto config = GetFuelControlSettings();

		if(config.settings.default_pumps_have_fuel) {
			return -1;
		}

		return 0;
	}
	
	bool HasFuel() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}
		
		if (group) {
			return group.HasFuel();
		}
		auto config = GetFuelControlSettings();

		return config.settings.default_pumps_have_fuel;
	}

	void RemoveFuel(float quantity) {
		if (group) {
			group.RemoveFuel(quantity);
		}
	}
	
	bool HasEnergy() {
		return m_working;
	}
	
	override bool IsRuined() {
		return m_ruined;
	}
	
	override void SetActions()
	{
		super.SetActions();
	}
};