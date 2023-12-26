

class FCAdminStationFormListItem: UIScriptedMenu {
	
	protected ref TextWidget m_name_txt;
	protected ref EditBoxWidget m_name_edit;
	protected ref TextWidget m_location_x_txt;
	protected ref TextWidget m_location_y_txt;
	protected ref TextWidget m_capacity_txt;
	protected ref TextWidget m_available_txt;
	ref FuelStationGroup m_station;
	
	void FCAdminStationFormListItem(FuelStationGroup station) {
		m_station = station;
	}
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form_list_item.layout");
		m_name_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("name_txt"));
		m_name_txt.SetText(m_station.name);
		m_name_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("name_edit"));
		

		m_capacity_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("capacity_txt"));
		m_available_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("available_txt"));
		m_location_x_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("location_x_txt"));
		m_location_y_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("location_y_txt"));

		UpdateUI();

		return layoutRoot;
	}
	
	void UpdateUI() {
		string capacity;
        if (m_station.fuelCapacity >= 0) {
            auto scaledCapacity = m_station.fuelCapacity / 1000;
            capacity = "" + scaledCapacity + "L";
        } else {
            capacity = "Infinite";
        }
		m_capacity_txt.SetText(capacity);
		
		string available;
        if (m_station.fuelAmount >= 0) {
            auto scaledAmount = m_station.fuelAmount / 1000;
            available = "" + scaledAmount + "L";
        } else {
            available = "Infinite";
        }
		m_available_txt.SetText(available);
		
		m_location_x_txt.SetText("" + m_station.position[0]);
		m_location_y_txt.SetText("" + m_station.position[2]);
	}
	
	void OnStationUpdate(FuelStationGroup station) {
		m_station = station;
		UpdateUI();
	}
	
}