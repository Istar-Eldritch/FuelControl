class FCAdminOtherForm: ScriptedWidgetEventHandler {
	
	ref Widget layoutRoot;
	protected ref CheckBoxWidget m_enable_spawn_check;
	protected ref EditBoxWidget m_spawn_interval_edit;
	protected ref EditBoxWidget m_spawn_amount_edit;
	protected ref EditBoxWidget m_spawn_minimum_edit;
	protected ref CheckBoxWidget m_pump_vehicle_refueling;
	protected ref CheckBoxWidget m_pump_barrel_refueling;
	protected ref CheckBoxWidget m_pump_siphoning;
	protected ref EditBoxWidget m_siphoning_limit_edit;
	protected ref CheckBoxWidget m_default_pumps_have_fuel;
	protected ref CheckBoxWidget m_pumps_get_ruined;
	protected ref CheckBoxWidget m_enable_measure_fuel_with_stick;
	protected ref CheckBoxWidget m_pumps_require_electricity;
	protected ref EditBoxWidget m_pump_consumption_edit;

	protected ref ButtonWidget m_submit;

	void FCAdminOtherForm(Widget parent) {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("FuelControl/GUI/layouts/fc_admin_other_form.layout", parent);
		layoutRoot.SetHandler(this);
		m_enable_spawn_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("spawn_check"));
		m_spawn_interval_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("spawn_interval_edit"));
		m_spawn_amount_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("spawn_amount_edit"));
		m_spawn_minimum_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("spawn_minimum_edit"));
		m_pump_vehicle_refueling = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("pump_vehicle_refueling"));
		m_pump_barrel_refueling = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("pump_barrel_refueling"));
		m_pump_siphoning = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("pump_siphoning"));
		m_siphoning_limit_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("siphoning_limit_edit"));
		m_default_pumps_have_fuel = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("default_pumps_have_fuel"));
		m_pumps_get_ruined = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("pumps_get_ruined"));
		m_enable_measure_fuel_with_stick = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("enable_measure_fuel_with_stick"));
		m_pumps_require_electricity = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("pumps_require_electricity"));
		m_pump_consumption_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("pump_consumption_edit"));
		m_submit = ButtonWidget.Cast(layoutRoot.FindAnyWidget("submit"));
		UpdateUI();
	}
	
	void UpdateUI() {
		Widget parent = layoutRoot.GetParent();
		float root_width;
		float root_height;
		parent.GetScreenSize(root_width, root_height);
		layoutRoot.SetScreenSize(root_width, root_height);
		auto settings = GetFuelControlSettings().settings;
		m_enable_spawn_check.SetChecked(settings.spawn_enabled);
		m_spawn_interval_edit.SetText("" + settings.spawn_interval);
		m_spawn_amount_edit.SetText("" + settings.spawn_amount);
		m_spawn_minimum_edit.SetText("" + settings.spawn_minimum);
		m_pump_vehicle_refueling.SetChecked(settings.pump_car_refueling);
		m_pump_barrel_refueling.SetChecked(settings.pump_barrel_refueling);
		m_pump_siphoning.SetChecked(settings.siphoning);
		m_siphoning_limit_edit.SetText("" + settings.siphoning_limit);
		m_default_pumps_have_fuel.SetChecked(settings.default_pumps_have_fuel);
		m_pumps_get_ruined.SetChecked(settings.pumps_get_ruined);
		m_pumps_require_electricity.SetChecked(settings.pumps_require_electricity);
		m_enable_measure_fuel_with_stick.SetChecked(settings.measure_fuel_with_stick);
		m_pump_consumption_edit.SetText("" + settings.pump_consumption);
	}
	
	override bool OnClick(Widget w, int x, int y, int button) {
		
		if (w == m_submit) {
			FuelControlSettings st = GetFuelControlSettings();
			auto settings = st.settings;
			bool new_spawn_enabled = m_enable_spawn_check.IsChecked();
			if (settings.spawn_enabled != new_spawn_enabled) {
				settings.spawn_enabled = new_spawn_enabled;
				st.SyncSetting("settings.spawn_enabled", "" + new_spawn_enabled);
			}
			int new_spawn_interval = m_spawn_interval_edit.GetText().ToInt();
			if (settings.spawn_interval != new_spawn_interval) {
				settings.spawn_interval = new_spawn_interval;
				st.SyncSetting("settings.spawn_interval", "" + new_spawn_interval);
			}
			int new_spawn_amount = m_spawn_amount_edit.GetText().ToInt();
			if (settings.spawn_amount != new_spawn_amount) {
				settings.spawn_amount = new_spawn_amount;
				st.SyncSetting("settings.spawn_amount", "" + new_spawn_amount);
			}
			int new_spawn_minimum = m_spawn_minimum_edit.GetText().ToInt();
			if (settings.spawn_minimum != new_spawn_minimum) {
				settings.spawn_minimum = new_spawn_minimum;
				st.SyncSetting("settings.spawn_minimum", "" + new_spawn_minimum);
			}
			
			bool new_pump_vehicle_refueling = m_pump_vehicle_refueling.IsChecked();
			if (settings.pump_car_refueling != new_pump_vehicle_refueling) {
				settings.pump_car_refueling = new_pump_vehicle_refueling;
				st.SyncSetting("settings.pump_car_refueling", "" + new_pump_vehicle_refueling);
			}
			
			bool new_pump_barrel_refueling = m_pump_barrel_refueling.IsChecked();
			if (settings.pump_barrel_refueling != new_pump_barrel_refueling) {
				settings.pump_barrel_refueling = new_pump_barrel_refueling;
				st.SyncSetting("settings.pump_barrel_refueling", "" + new_pump_barrel_refueling);
			}

			bool new_siphoning = m_pump_siphoning.IsChecked();
			if (settings.siphoning != new_siphoning) {
				settings.siphoning = new_siphoning;
				st.SyncSetting("settings.siphoning", "" + new_siphoning);
			}

			float new_siphoning_limit = m_siphoning_limit_edit.GetText().ToFloat();
			if (!IE_CmpFloats(settings.siphoning_limit, new_siphoning_limit)) {
				settings.siphoning_limit = new_siphoning_limit;
				st.SyncSetting("settings.siphoning_limit", "" + new_siphoning_limit);
			}
			
			bool new_default_pumps_have_fuel = m_default_pumps_have_fuel.IsChecked();
			if (settings.default_pumps_have_fuel != new_default_pumps_have_fuel) {
				settings.default_pumps_have_fuel = new_default_pumps_have_fuel;
				st.SyncSetting("settings.default_pumps_have_fuel", "" + new_default_pumps_have_fuel);
			}
			
			bool new_pumps_get_ruined = m_pumps_get_ruined.IsChecked();
			if (settings.pumps_get_ruined != new_pumps_get_ruined) {
				settings.pumps_get_ruined = new_pumps_get_ruined;
				st.SyncSetting("settings.pumps_get_ruined", "" + new_pumps_get_ruined);
			}
			
			bool new_enable_measure_fuel_with_stick = m_enable_measure_fuel_with_stick.IsChecked();
			if (settings.measure_fuel_with_stick != new_enable_measure_fuel_with_stick) {
				settings.measure_fuel_with_stick = new_enable_measure_fuel_with_stick;
				st.SyncSetting("settings.measure_fuel_with_stick", "" + new_enable_measure_fuel_with_stick);
			}

			bool new_pumps_require_electricity = m_pumps_require_electricity.IsChecked();
			if (settings.pumps_require_electricity != new_pumps_require_electricity) {
				settings.pumps_require_electricity = new_pumps_require_electricity;
				st.SyncSetting("settings.pumps_require_electricity", "" + new_pumps_require_electricity);
			}

			int new_pump_consumption = m_pump_consumption_edit.GetText().ToInt();
			if (settings.pump_consumption != new_pump_consumption) {
				settings.pump_consumption = new_pump_consumption;
				st.SyncSetting("settings.pump_consumption", "" + new_pump_consumption);
			}
			
		}
		
		return false;
	}
}