class FCAdminStationFormSubscriber: StationSubscriber {
	
	FCAdminStationForm m_form;
	bool m_pending_update;
	
	void FCAdminStationFormSubscriber(FCAdminStationForm form) {
		m_form = form;
		m_pending_update = false;
	}
	
	override void OnUpdate(FuelStationGroup station) {
		if (!m_pending_update) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateUI, 200);
			m_pending_update = true;
		}
	}
	
	override void OnDelete(FuelStationGroup station) {
		if (!m_pending_update) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateUI, 200);
			m_pending_update = true;
		}
	}
	
	void UpdateUI() {
		m_form.UpdateUI();
		m_pending_update = false;
	}
}

const int FC_COLOR_RED = ARGB( 255, 200, 20, 20 );
const int FC_COLOR_GREEN = ARGB( 255, 20, 200, 20 );
const int FC_COLOR_ORANGE = ARGB( 255, 255, 165, 0 );
const int FC_COLOR_BLUE = ARGB( 255, 20, 20, 200 );

class FCSelectStationSubscriber {
	
	FCAdminStationForm m_form;
	
	void FCSelectStationSubscriber(FCAdminStationForm form) {
		m_form = form;
	}
	
	void ~FCSelectStationSubscriber() {
		Print("Removing subscriber");
	}
	
	void OnSelect(FuelStationGroup m_station) {
		m_form.m_selectedStation = m_station;
		m_form.UpdateSelected();
	}
	
	void OnOptions(FuelStationGroup m_station, int x, int y) {}
	
	void OnCreate(FuelStationGroup m_station) {
		delete m_form.m_newStation;
		m_form.m_selectedStation = m_station;
		m_form.UpdateUI();
	}
	
	void OnCreateCancel() {
		delete m_form.m_newStation;
		delete m_form.m_selectedStation;
		m_form.UpdateUI();
	}
}

class FCAdminStationForm: ScriptedWidgetEventHandler {
	
	protected ref ScrollWidget m_stationScroll;
	protected ref Widget m_stationList;
	protected ref Widget m_rightPanel;
	protected ref MapWidget m_stationMap;
	
	protected ref EditBoxWidget m_filterBox;
	protected ref ButtonWidget m_newBtn;
	
	ref FuelStationManager m_stationManager;
	ref FCAdminStationFormSubscriber m_subscriber;
	
	ref array<FuelStationGroup> m_stations;
	ref array<ref FCAdminStationFormListItem> m_children;
	FuelStationGroup m_selectedStation;
	ref FCSelectStationSubscriber m_selectedStationSubscriber;
	Widget layoutRoot;
	
	int m_lastUIUpdateTime = 0;
	string m_lastFilter = "";
	bool m_stopChecks = false;
	ref	FCAdminStationFormListItem m_newStationWidget;
	ref FuelStationGroup m_newStation;

	void FCAdminStationForm(Widget parent) {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form.layout", parent);
		layoutRoot.SetHandler(this);
		m_selectedStationSubscriber = new FCSelectStationSubscriber(this);
		OnInit();
	}
	
	void ~FCAdminStationForm() {
		m_stopChecks = true;
		m_stationManager.DeSubscribe(m_subscriber);
		delete m_selectedStationSubscriber;
		delete m_subscriber;
		delete m_children;
		delete layoutRoot;
	}
	
	void OnInit() {
		m_children = new array<ref FCAdminStationFormListItem>;
		m_stationManager = GetFuelStationManager();
		m_stations = m_stationManager.m_stations.GetValueArray();
		m_subscriber = FCAdminStationFormSubscriber(this);
		m_stationManager.Subscribe(m_subscriber);

		m_stationScroll = ScrollWidget.Cast(layoutRoot.FindAnyWidget("station_scroll"));
		m_stationScroll.VScrollToPos(0);
		m_stationList = layoutRoot.FindAnyWidget("station_list");
		
		m_rightPanel = layoutRoot.FindAnyWidget("right_panel");

		m_stationMap = MapWidget.Cast(layoutRoot.FindAnyWidget("station_map"));
		
		m_newBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("new_btn"));
		m_filterBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("filter_box"));

		CheckLoop();
	}
	
	void FilterStations() {
		string filter = m_filterBox.GetText();
		filter.ToLower();
		if (filter != "") {
			auto newArray = new array<FuelStationGroup>;
			foreach (auto station: m_stations) {
				string name = station.name;
				name.ToLower();
				if (name.Contains(filter)) {
					newArray.Insert(station);
				}
			}
			m_stations = newArray;
		}
	}
	
	void SortStations() {
		bool swapped = true;
		int counter = 0;
		int swaps = 0;
		while (swapped) {          
		     while (counter < m_stations.Count() - 1) {
				auto current = m_stations.Get(counter);
				auto next = m_stations.Get(counter + 1);
		     	if (current.name > next.name) {
					m_stations.Set(counter, next);
					m_stations.Set(counter + 1, current);
	                swaps = swaps + 1;
				}
		        counter = counter + 1;
			}
		    if (swaps == 0) {
		         swapped = false;
		    } else {
		         swaps = 0;
		         counter = 0;
			}
		}
	}
	
	void UpdateSelected() {
		if (m_selectedStation) {
			m_stationMap.SetMapPos(m_selectedStation.position);
		} else if (m_stations.Count() > 0) {
			m_stationMap.SetMapPos(m_stations.Get(0).position);
		}
	}

	void UpdateUI() {
		float root_width;
		float root_height;
		Widget parent = layoutRoot.GetParent();
		parent.GetScreenSize(root_width, root_height);
		layoutRoot.SetSize(root_width, root_height);
		layoutRoot.SetPos(0, 0);
		float scroll_width;
		float scroll_height;
		m_stationScroll.GetScreenSize(scroll_width, scroll_height);
		m_stationScroll.SetScreenSize(scroll_width, root_height);
		auto map_width = root_width - scroll_width;
		m_rightPanel.SetScreenSize(map_width, root_height);
		m_rightPanel.SetPos(scroll_width, 0);
		m_stationMap.SetScreenSize(map_width, root_height);
		
		foreach (auto child: m_children) {
			if (child) {
				m_stationList.RemoveChild(child.layoutRoot);
			}
		}
		delete m_children;
		m_children = new array<ref FCAdminStationFormListItem>;

		m_stationMap.ClearUserMarks();
		
		float item_width;
		float item_height;
		if (m_newStation) {
			m_newStationWidget = new FCAdminStationFormListItem(m_stationList, m_newStation, m_selectedStationSubscriber);
			m_newStationWidget.layoutRoot.GetScreenSize(item_width, item_height);
			m_newStationWidget.m_currentState = FCAdminStationFormListItem.STATE_EDIT;
			m_newStationWidget.UpdateUI();
			m_newBtn.SetFlags(WidgetFlags.DISABLED);
			m_filterBox.SetFlags(WidgetFlags.DISABLED);
			m_children.Insert(m_newStationWidget);
			m_newStationWidget.layoutRoot.SetPos(0, 0);
			m_stationMap.AddUserMark(m_newStation.position, m_newStation.name, FC_COLOR_GREEN, "\\FuelControl\\GUI\\textures\\gas_station_green.paa");
		} else {
			m_newBtn.ClearFlags(WidgetFlags.DISABLED);
			m_filterBox.ClearFlags(WidgetFlags.DISABLED);
		}

		m_stations = m_stationManager.m_stations.GetValueArray();
		FilterStations();
		SortStations();
		foreach (auto station: m_stations) {
			auto item = new FCAdminStationFormListItem(m_stationList, station, m_selectedStationSubscriber);
			m_children.Insert(item);
			item.layoutRoot.GetScreenSize(item_width, item_height);
			float item_y_pos = item_height * (m_children.Count() - 1);
			item.layoutRoot.SetPos(0, item_y_pos);
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
		m_stationList.SetSize(item_width, item_height * m_children.Count() + item_height / 2);
		m_stationList.SetPos(0, 0);
		m_stationList.Show(true);
		UpdateSelected();
	}
	
	void CheckLoop() {
		int currentTime = GetGame().GetTime();
		string filter = m_filterBox.GetText();
		if (m_lastFilter != filter && (currentTime - m_lastUIUpdateTime) > 200) { 
			m_lastUIUpdateTime = currentTime;
			m_lastFilter = filter;
			UpdateUI();
		}
		if (!m_stopChecks) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CheckLoop, 200);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_newBtn) {
			string id;
			DayZPlayer player = GetGame().GetPlayer();
			vector position = player.GetWorldPosition();
			m_newStation = new FuelStationGroup(id, "New station", position, -1, -1);
			m_selectedStation = m_newStation;
			m_filterBox.SetText("");
			UpdateUI();
			return true;
		}
		return false;
	}
}