class IE_FC_StationPowerSource: BuildingSuper {
	
	static const string	LOOP_SOUND = "IE_FC_ElectricalBoxOnLoop_SoundSet";

	protected EffectSound 	m_HumLoop;
	protected bool m_Working = false;
	
	void IE_FC_StationPowerSource() {
		RegisterNetSyncVariableBool("m_Working");
	}
	
	override void OnWorkStart() {
		super.OnWorkStart();
		m_Working = true;
		SetSynchDirty();
	}
	
	override void OnWorkStop() {
		super.OnWorkStop();
		m_Working = false;
		SetSynchDirty();
	}
	
	override void OnVariablesSynchronized() {
		super.OnVariablesSynchronized();
		if (GetGame().IsClient()) {
			if (m_Working) {
				PlaySoundSetLoop(m_HumLoop, LOOP_SOUND, 0, 0);
			} else {
				StopSoundSet(m_HumLoop);
			}
		}
	}
	
	override bool IsInventoryVisible() {
		return true;
	}
	
	override bool IsHealthVisible() {
		return true;
	}
	
	override bool IsElectricAppliance() {
		return true;
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(IEActionTurnOn);
		RemoveAction(IEActionTurnOff);
	}
}

class IE_FC_ElectricalBox: IE_FC_StationPowerSource {}