class FCAdminStationFormSubscriber: StationSubscriber {
	
	FCAdminStationForm m_form;
	bool m_pending_update;
	
	void FCAdminStationFormSubscriber(FCAdminStationForm form) {
		m_form = form;
		m_pending_update = false;
	}
	
	override void OnUpdate(FuelStationGroup station) {
		m_form.UpdateUI(station);
	}
	
	void UpdateUI(FuelStationGroup station) {
		m_form.UpdateUI(station);
	}
}

const int FC_COLOR_RED = ARGB( 255, 200, 20, 20 );
const int FC_COLOR_GREEN = ARGB( 255, 20, 200, 20 );
const int FC_COLOR_ORANGE = ARGB( 255, 255, 165, 0 );
const int FC_COLOR_BLUE = ARGB( 255, 20, 20, 200 );

class FCAdminStationForm: UIScriptedMenu {
	
	protected ref ScrollWidget m_stationScroll;
	protected ref Widget m_stationList;
	protected ref Widget m_rightPanel;
	protected ref MapWidget m_stationMap;
	
	ref FuelStationManager m_stationManager;
	ref FCAdminStationFormSubscriber m_subscriber;
	
	ref map<string, FCAdminStationFormListItem> m_children;
	
	float m_height;
	float m_width;
	
	void FCAdminStationForm(float width = -1, float height = -1) {
		if (width > 0 && height > 0) {
			m_width = width;
			m_height = height;
		}
		m_children = new map<string, FCAdminStationFormListItem>;
		m_stationManager = GetFuelStationManager();
		m_subscriber = FCAdminStationFormSubscriber(this);
		m_stationManager.Subscribe(m_subscriber);
	}
	
	void ~FCAdminStationForm() {
		m_stationManager.DeSubscribe(m_subscriber);
	}
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form.layout");
		if(!m_width && !m_height) {
			layoutRoot.GetScreenSize(m_width, m_height);
		}
		m_stationScroll = ScrollWidget.Cast(layoutRoot.FindAnyWidget("station_scroll"));
		float scroll_width;
		float scroll_height;
		m_stationScroll.GetScreenSize(scroll_width, scroll_height);
		m_stationScroll.VScrollToPos(0);
		m_stationList = layoutRoot.FindAnyWidget("station_list");
		
		auto map_width = m_width - scroll_width - 30;
		auto map_height = m_height - 20;
		m_rightPanel = layoutRoot.FindAnyWidget("right_panel");
		m_rightPanel.SetScreenSize(map_width, map_height);
		m_rightPanel.SetPos(scroll_width + 10, 0);
		m_stationMap = MapWidget.Cast(layoutRoot.FindAnyWidget("station_map"));
		m_stationMap.SetScreenSize(map_width, map_height);
		m_stationMap.SetPos(0, 0);
		m_stationManager.SendRequestAllStations();
		return layoutRoot;
	}

	void UpdateUI(FuelStationGroup station) {
		if (station) {
			auto item = m_children.Get(station.name);
			if (item) {
				item.OnStationUpdate(station);
			} else {
				item = new FCAdminStationFormListItem(station);
				Widget item_widget = item.Init();
				float item_width;
				float item_height;
				item_widget.GetSize(item_width, item_height);
				item_widget.SetPos(0, item_height * m_children.Count());
				m_stationList.AddChild(item_widget);
				m_children.Insert(station.name, item);
				m_stationList.SetSize(item_width, item_height * m_children.Count());
				if (station.fuelAmount < 0) {
					m_stationMap.AddUserMark(station.position, station.name, FC_COLOR_BLUE, "\\FuelControl\\GUI\\textures\\gas_station_blue.paa");
				} else {
					auto ratio = station.fuelAmount / station.fuelCapacity;
					if (ratio > 0.5) {
						m_stationMap.AddUserMark(station.position, station.name, FC_COLOR_GREEN, "\\FuelControl\\GUI\\textures\\gas_station_green.paa");	
					} else if (ratio > 0.2) {
						m_stationMap.AddUserMark(station.position, station.name, FC_COLOR_ORANGE, "\\FuelControl\\GUI\\textures\\gas_station_orange.paa");	
					} else {
						m_stationMap.AddUserMark(station.position, station.name, FC_COLOR_RED, "\\FuelControl\\GUI\\textures\\gas_station_red.paa");	
					}
				}
			}
		}
	}
}