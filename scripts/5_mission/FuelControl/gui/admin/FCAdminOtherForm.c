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
		m_submit = ButtonWidget.Cast(layoutRoot.FindAnyWidget("submit"));
		GetFuelControlSettings().SyncSettings();
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
		m_enable_measure_fuel_with_stick.SetChecked(settings.measure_fuel_with_stick);
	}
	
	override bool OnClick(Widget w, int x, int y, int button) {
		
		if (w == m_submit) {
			FuelControlSettings set_manager = GetFuelControlSettings();
			auto settings = set_manager.settings;
			settings.spawn_enabled = m_enable_spawn_check.IsChecked();
			settings.spawn_interval = m_spawn_interval_edit.GetText().ToInt();
			settings.spawn_amount = m_spawn_amount_edit.GetText().ToInt();
			settings.spawn_minimum = m_spawn_minimum_edit.GetText().ToInt();
			settings.pump_car_refueling = m_pump_vehicle_refueling.IsChecked();
			settings.pump_barrel_refueling = m_pump_barrel_refueling.IsChecked();
			settings.siphoning = m_pump_siphoning.IsChecked();
			settings.siphoning_limit = m_siphoning_limit_edit.GetText().ToFloat();
			settings.default_pumps_have_fuel = m_default_pumps_have_fuel.IsChecked();
			settings.pumps_get_ruined = m_pumps_get_ruined.IsChecked();
			settings.measure_fuel_with_stick = m_enable_measure_fuel_with_stick.IsChecked();
			
			set_manager.settings = settings;
			set_manager.SyncSettings(true);
			
		}
		
		return false;
	}
}