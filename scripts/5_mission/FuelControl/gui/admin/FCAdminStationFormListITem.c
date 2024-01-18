
class FCAdminStationFormListItem: ScriptedWidgetEventHandler {
	
	protected ref Widget m_infoPanel;
	protected ref TextWidget m_name_txt;
	protected ref TextWidget m_location_x_txt;
	protected ref TextWidget m_location_y_txt;
	protected ref TextWidget m_gasoline_capacity_txt;
	protected ref TextWidget m_gasoline_available_txt;
	protected ref TextWidget m_diesel_capacity_txt;
	protected ref TextWidget m_diesel_available_txt;
	protected ref TextWidget m_avgas_capacity_txt;
	protected ref TextWidget m_avgas_available_txt;
	protected ref ButtonWidget m_tp_btn;
	protected ref ButtonWidget m_edit_btn;
	protected ref ButtonWidget m_del_btn;
	
	protected ref Widget m_editPanel;
	protected ref EditBoxWidget m_name_edit;
	protected ref EditBoxWidget m_location_x_edit;
	protected ref EditBoxWidget m_location_y_edit;
	protected ref EditBoxWidget m_gasoline_capacity_edit;
	protected ref EditBoxWidget m_gasoline_available_edit;
	protected ref EditBoxWidget m_diesel_capacity_edit;
	protected ref EditBoxWidget m_diesel_available_edit;
	protected ref EditBoxWidget m_avgas_capacity_edit;
	protected ref EditBoxWidget m_avgas_available_edit;
	protected ref ButtonWidget m_ok_edit_btn;
	protected ref ButtonWidget m_cancel_edit_btn;
	
	protected ref Widget m_delPanel;
	protected ref TextWidget m_del_txt;
	protected ref ButtonWidget m_ok_del_btn;
	protected ref ButtonWidget m_cancel_del_btn;

	ref FuelStationGroup m_station;
	ref Widget layoutRoot;
	ref FCSelectStationSubscriber m_onSelectSubscriber;
	
	const int STATE_INFO = 0;
	const int STATE_EDIT = 1;
	const int STATE_DEL = 2;
	
	int m_currentState = STATE_INFO;
	
	void FCAdminStationFormListItem(Widget parent, FuelStationGroup station, FCSelectStationSubscriber onSelectSubscriber = null) {
		m_station = station;
		m_onSelectSubscriber = onSelectSubscriber;
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form_list_item.layout", parent);
		layoutRoot.SetHandler(this);
		OnInit();
	}
	
	void ~FCAdminStationFormListItem() {
		delete layoutRoot;
	}
	
	void OnInit() {
		m_infoPanel = layoutRoot.FindAnyWidget("info_panel");
		m_name_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("name_txt"));
		m_gasoline_capacity_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("gasoline_capacity_txt"));
		m_gasoline_available_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("gasoline_available_txt"));
		m_diesel_capacity_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("diesel_capacity_txt"));
		m_diesel_available_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("diesel_available_txt"));
		m_avgas_capacity_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("avgas_capacity_txt"));
		m_avgas_available_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("avgas_available_txt"));
		m_location_x_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("location_x_txt"));
		m_location_y_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("location_y_txt"));
		m_tp_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("tp_btn"));
		m_edit_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("edit_btn"));
		m_del_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("del_btn"));
		
		m_editPanel = layoutRoot.FindAnyWidget("edit_panel");
		m_name_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("name_edit"));
		m_gasoline_capacity_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("gasoline_capacity_edit"));
		m_gasoline_available_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("gasoline_available_edit"));
		m_diesel_capacity_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("diesel_capacity_edit"));
		m_diesel_available_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("diesel_available_edit"));
		m_avgas_capacity_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("avgas_capacity_edit"));
		m_avgas_available_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("avgas_available_edit"));		
		m_location_x_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("location_x_edit"));
		m_location_y_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("location_y_edit"));
		m_ok_edit_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ok_edit_btn"));
		m_cancel_edit_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_edit_btn"));
		
		m_delPanel = layoutRoot.FindAnyWidget("del_panel");
		m_del_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("del_txt"));
		m_ok_del_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ok_del_btn"));
		m_cancel_del_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_del_btn"));

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(UpdateUI);
	}
	
	void _SetupInfoPanel() {
		m_infoPanel.Show(true);
		m_name_txt.SetText(m_station.m_config.name);

		string gasolineCapacity;
        if (m_station.GetCapacity(LIQUID_GASOLINE) >= 0) {
            auto scaledGasolineCapacity = m_station.GetCapacity(LIQUID_GASOLINE);
            gasolineCapacity = "" + scaledGasolineCapacity + "L";
        } else {
            gasolineCapacity = "Infinite";
        }
		m_gasoline_capacity_txt.SetText(gasolineCapacity);

		string gasolineAvailable;
        if (m_station.GetFuel(LIQUID_GASOLINE) >= 0) {
            auto scaledGasolineAmount = m_station.GetFuel(LIQUID_GASOLINE);
            gasolineAvailable = "" + scaledGasolineAmount + "L";
        } else {
            gasolineAvailable = "Infinite";
        }
		m_gasoline_available_txt.SetText(gasolineAvailable);
		
		string dieselCapacity;
        if (m_station.GetCapacity(LIQUID_DIESEL) >= 0) {
            auto scaledDieselCapacity = m_station.GetCapacity(LIQUID_DIESEL);
            dieselCapacity = "" + scaledDieselCapacity + "L";
        } else {
            dieselCapacity = "Infinite";
        }
		m_diesel_capacity_txt.SetText(dieselCapacity);
		
		string dieselAvailable;
        if (m_station.GetFuel(LIQUID_DIESEL) >= 0) {
            auto scaledDieselAmount = m_station.GetFuel(LIQUID_DIESEL);
            dieselAvailable = "" + scaledDieselAmount + "L";
        } else {
            dieselAvailable = "Infinite";
        }
		m_diesel_available_txt.SetText(dieselAvailable);
		
		string avgasCapacity;
        if (m_station.GetCapacity(IE_FC_LIQUID_AVGAS) >= 0) {
            auto scaledAvgasCapacity = m_station.GetCapacity(IE_FC_LIQUID_AVGAS);
            avgasCapacity = "" + scaledAvgasCapacity + "L";
        } else {
            avgasCapacity = "Infinite";
        }
		m_avgas_capacity_txt.SetText(avgasCapacity);
		
		string avgasAvailable;
        if (m_station.GetFuel(IE_FC_LIQUID_AVGAS) >= 0) {
            auto scaledAvgasAmount = m_station.GetFuel(IE_FC_LIQUID_AVGAS);
            avgasAvailable = "" + scaledAvgasAmount + "L";
        } else {
            avgasAvailable = "Infinite";
        }
		m_avgas_available_txt.SetText(avgasAvailable);
		
		m_location_x_txt.SetText("" + m_station.m_config.x);
		m_location_y_txt.SetText("" + m_station.m_config.y);
	}
	
	void _SetupEditPanel() {
		m_editPanel.Show(true);
		m_name_edit.SetText(m_station.m_config.name);
		
		string gasolineCapacity;
        if (m_station.GetCapacity(LIQUID_GASOLINE) >= 0) {
            auto scaledGasolineCapacity = m_station.GetCapacity(LIQUID_GASOLINE);
            gasolineCapacity = "" + scaledGasolineCapacity;
        } else {
            gasolineCapacity = "-1";
        }
		m_gasoline_capacity_edit.SetText(gasolineCapacity);

		string gasolineAvailable;
        if (m_station.GetFuel(LIQUID_GASOLINE) >= 0) {
            auto scaledGasolineAmount = m_station.GetFuel(LIQUID_GASOLINE);
            gasolineAvailable = "" + scaledGasolineAmount;
        } else {
            gasolineAvailable = "-1";
        }
		m_gasoline_available_edit.SetText(gasolineAvailable);
		
		string dieselCapacity;
        if (m_station.GetCapacity(LIQUID_DIESEL) >= 0) {
            auto scaledDieselCapacity = m_station.GetCapacity(LIQUID_DIESEL);
            dieselCapacity = "" + scaledDieselCapacity;
        } else {
            dieselCapacity = "-1";
        }
		m_diesel_capacity_edit.SetText(dieselCapacity);
		
		string dieselAvailable;
        if (m_station.GetFuel(LIQUID_DIESEL) >= 0) {
            auto scaledDieselAmount = m_station.GetFuel(LIQUID_DIESEL);
            dieselAvailable = "" + scaledDieselAmount;
        } else {
            dieselAvailable = "-1";
        }
		m_diesel_available_edit.SetText(dieselAvailable);
		
		string avgasCapacity;
        if (m_station.GetCapacity(IE_FC_LIQUID_AVGAS) >= 0) {
            auto scaledAvgasCapacity = m_station.GetCapacity(IE_FC_LIQUID_AVGAS);
            avgasCapacity = "" + scaledAvgasCapacity;
        } else {
            avgasCapacity = "-1";
        }
		m_avgas_capacity_edit.SetText(avgasCapacity);
		
		string avgasAvailable;
        if (m_station.GetFuel(IE_FC_LIQUID_AVGAS) >= 0) {
            auto scaledAvgasAmount = m_station.GetFuel(IE_FC_LIQUID_AVGAS);
            avgasAvailable = "" + scaledAvgasAmount;
        } else {
            avgasAvailable = "-1";
        }
		m_avgas_available_edit.SetText(avgasAvailable);

		m_location_x_edit.SetText("" + m_station.m_config.x);
		m_location_y_edit.SetText("" + m_station.m_config.y);
	}
	
	void _SetupDelPanel() {
		m_delPanel.Show(true);
		m_del_txt.SetText("Delete " + m_station.m_config.name + "?");
	}
	
	void UpdateUI() {
		float root_width;
		float root_height;
		Widget parent = layoutRoot.GetParent();
		parent.GetScreenSize(root_width, root_height);
		float current_width;
		float current_height;
		layoutRoot.GetScreenSize(current_width, current_height);
		//layoutRoot.SetSize(root_width, current_height);
		m_infoPanel.Show(false);
		m_editPanel.Show(false);
		m_delPanel.Show(false);
		if (m_currentState == STATE_INFO) {
			_SetupInfoPanel();
		} else if (m_currentState == STATE_EDIT) {
			_SetupEditPanel();
		} else if (m_currentState == STATE_DEL) {
			_SetupDelPanel();
		}
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button) {
		if (button == 0) {
			if (w == m_tp_btn) {
				FCGetTeleportManager().SendTeleportToStation(m_station);
			} else if (w == m_edit_btn) {
				m_currentState = STATE_EDIT;
				UpdateUI();
			} else if (w == m_del_btn) {
				m_currentState = STATE_DEL;
				UpdateUI();
			} else if (w == m_cancel_edit_btn) {
				if (!m_station.m_config.id) {
					m_onSelectSubscriber.OnCreateCancel();
				}
				m_currentState = STATE_INFO;
				UpdateUI();
			} else if (w == m_ok_edit_btn) {
				bool isNew = false;
				if (!m_station.m_config.id) {
					m_station.m_config.id = FuelStationManager.GenId(m_station.m_config.name);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.id", m_station.m_config.id, true);
					isNew = true;
				}
				string name = m_name_edit.GetText();
				if (m_station.m_config.name != name) {
					m_station.m_config.name = name;
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.name", name, true);
				}
				float xx = m_location_x_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.m_config.x, xx)) {
					m_station.m_config.x = xx;
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.x", m_location_x_edit.GetText(), true);
				}
				float yy = m_location_y_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.m_config.y, yy)) {
					m_station.m_config.y = yy;
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.y", m_location_y_edit.GetText(), true);
				}
				float gasoline_capacity = m_gasoline_capacity_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.GetCapacity(LIQUID_GASOLINE), gasoline_capacity)) {
					m_station.SetCapacity(LIQUID_GASOLINE, gasoline_capacity);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.fuels." + LIQUID_GASOLINE + ".capacity",  m_gasoline_capacity_edit.GetText(), true);
				}
				float diesel_capacity = m_diesel_capacity_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.GetCapacity(LIQUID_DIESEL), diesel_capacity)) {
					m_station.SetCapacity(LIQUID_DIESEL, diesel_capacity);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.fuels." + LIQUID_DIESEL + ".capacity",  m_diesel_capacity_edit.GetText(), true);
				}
				float avgas_capacity = m_avgas_capacity_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.GetCapacity(IE_FC_LIQUID_AVGAS), avgas_capacity)) {
					m_station.SetCapacity(LIQUID_GASOLINE, avgas_capacity);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "config.fuels." + IE_FC_LIQUID_AVGAS + ".capacity",  m_avgas_capacity_edit.GetText(), true);
				}
				float gasoline_available = m_gasoline_available_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.GetFuel(LIQUID_GASOLINE), gasoline_available)) {
					m_station.SetFuel(LIQUID_GASOLINE, gasoline_available);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "state.fuels." + LIQUID_GASOLINE + ".available",  m_gasoline_available_edit.GetText(), true);
				}
				float diesel_available = m_diesel_available_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.GetFuel(LIQUID_DIESEL), gasoline_available)) {
					m_station.SetFuel(LIQUID_DIESEL, gasoline_available);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "state.fuels." + LIQUID_DIESEL + ".available",  m_diesel_available_edit.GetText(), true);
				}
				float avgas_available = m_avgas_available_edit.GetText().ToFloat();
				if (!IE_CmpFloats(m_station.GetFuel(IE_FC_LIQUID_AVGAS), avgas_available)) {
					m_station.SetFuel(IE_FC_LIQUID_AVGAS, avgas_available);
					GetFuelStationManager().UpdateStation(m_station.m_config.id, "state.fuels." + IE_FC_LIQUID_AVGAS + ".available",  m_avgas_available_edit.GetText(), true);
				}

				if (isNew) {
					m_onSelectSubscriber.OnCreate(m_station);
				} else {
					m_currentState = STATE_INFO;
					UpdateUI();
				}
			} else if (w == m_cancel_del_btn) {
				m_currentState = STATE_INFO;
				UpdateUI();
			} else if (w == m_ok_del_btn) {
				GetFuelStationManager().DeleteStation(m_station.m_config.id, true);
			} else if (m_onSelectSubscriber) {
				m_onSelectSubscriber.OnSelect(m_station);
			}
			return true;
		} else if (button == 1) {
			m_onSelectSubscriber.OnOptions(m_station, x, y);
			return true;
		}
		return false;
	}
	
}