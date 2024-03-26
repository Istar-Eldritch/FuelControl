class FCAdminForm: ScriptedWidgetEventHandler {
	
	protected ref Widget layoutRoot;
	protected ref Widget m_content;
    protected ref Widget m_tab_panel;
	protected ref ButtonWidget m_settings_btn;
	protected ref ButtonWidget m_stations_btn;
	protected ref ButtonWidget m_consumption_btn;
	protected ref ButtonWidget m_transfer_btn;
	protected ref ButtonWidget m_powerbox_btn;
	protected ref FCAdminStationForm m_stationForm;
	protected ref FCAdminOtherForm m_otherForm;
	protected ref FCAdminVehicleForm m_vehicleForm;
	protected ref FCAdminTransferRatesForm m_transferRatesForm;
	protected ref FCAdminPowerBoxForm m_powerBoxForm;
	
	const int SELECT_STATIONS = 0;
	const int SELECT_SETTINGS = 1;
	const int SELECT_CONSUMPTION = 2;
	const int SELECT_TRANSFER = 3;
	const int SELECT_POWER_BOXES = 4;
	
	int m_selectedTab = SELECT_STATIONS;
	
	void FCAdminForm(Widget parent) {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/data/layouts/fc_admin_form.layout", parent);
		layoutRoot.SetHandler(this);
		OnInit();
	}
	
	void OnInit() {
        m_tab_panel = layoutRoot.FindAnyWidget("tab_panel");
		m_content = layoutRoot.FindAnyWidget("content");
		m_stationForm = new FCAdminStationForm(m_content);
		m_otherForm = new FCAdminOtherForm(m_content);
		m_vehicleForm = new FCAdminVehicleForm(m_content);
		m_transferRatesForm = new FCAdminTransferRatesForm(m_content);
		m_powerBoxForm = new FCAdminPowerBoxForm(m_content);
		m_stations_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("stations_btn"));
		m_settings_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("settings_btn"));
		m_consumption_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("consumption_btn"));
		m_transfer_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("transfer_btn"));
		m_powerbox_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("powerbox_btn"));
		UpdateUI();
	}
	
	void UpdateUI() {
		float root_width;
		float root_height;
		layoutRoot.GetParent().GetScreenSize(root_width, root_height);
		root_width = root_width - 20;
		root_height = root_height - 20;
		layoutRoot.SetSize(root_width, root_height);
		layoutRoot.SetPos(10, 10);
		float tab_width;
        float tab_height;
		m_tab_panel.GetScreenSize(tab_width, tab_height);
		m_content.SetSize(root_width, root_height - tab_height);
		m_content.SetPos(0, tab_height);
		
		m_stationForm.layoutRoot.Show(false);
		m_otherForm.layoutRoot.Show(false);
		m_vehicleForm.layoutRoot.Show(false);
		m_transferRatesForm.layoutRoot.Show(false);
		m_powerBoxForm.layoutRoot.Show(false);

		switch (m_selectedTab) {
			case SELECT_STATIONS:
				m_stationForm.layoutRoot.Show(true);
				m_stationForm.UpdateUI();
				break;
			case SELECT_SETTINGS:
				m_otherForm.layoutRoot.Show(true);
				m_otherForm.UpdateUI();
				break;
			case SELECT_CONSUMPTION:
				m_vehicleForm.layoutRoot.Show(true);
				m_vehicleForm.UpdateUI();
				break;
			case SELECT_TRANSFER:
				m_transferRatesForm.layoutRoot.Show(true);
				m_transferRatesForm.UpdateUI();
				break;
			case SELECT_POWER_BOXES:
				m_powerBoxForm.layoutRoot.Show(true);
				m_powerBoxForm.UpdateUI();
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_settings_btn) {
			m_selectedTab = SELECT_SETTINGS;
		} else if (w == m_stations_btn) {
			m_selectedTab = SELECT_STATIONS;
		} else if (w == m_consumption_btn) {
			m_selectedTab = SELECT_CONSUMPTION;
		} else if (w == m_transfer_btn) {
			m_selectedTab = SELECT_TRANSFER;
		} else if (w == m_powerbox_btn) {
			m_selectedTab = SELECT_POWER_BOXES;
		} else {
            return false;
        }
		UpdateUI();
        return true;
	}
}