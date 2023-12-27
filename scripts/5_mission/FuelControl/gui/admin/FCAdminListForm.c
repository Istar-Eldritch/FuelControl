class FCKV {
	string key;
	string value;
}

class FCAdminListItemForm: ScriptedWidgetEventHandler {
	ref Widget layoutRoot;
	protected ref EditBoxWidget m_key_box;
	protected ref EditBoxWidget m_value_box;
	protected ref ButtonWidget m_delete;
	ref FCKV m_o;
	protected ref FCAdminListForm m_handler;
	int m_id;
	
	void FCAdminListItemForm(Widget parent, FCAdminListForm handler, int id, FCKV o) {
		m_o = o;
		m_id = id;
		m_handler = handler;
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("FuelControl/GUI/layouts/fc_admin_key_value.layout", parent);
		layoutRoot.SetHandler(this);
		m_key_box = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("key"));
		m_key_box.SetText(o.key);
		m_value_box = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("value"));
		m_value_box.SetText(o.value);
		m_delete = ButtonWidget.Cast(layoutRoot.FindAnyWidget("del_btn"));
	}
	
	string GetKey() {
		return m_key_box.GetText();
	}
	
	string GetValue() {
		return m_value_box.GetText();
	}
	
	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_delete) {
			m_handler.OnDelete(this);
			return true;
		}
        return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		m_handler.OnUpdate(this);
		return false;
	}
}

class FCAdminListForm: ScriptedWidgetEventHandler {
	
	ref Widget layoutRoot;
	protected ref Widget m_scroll;
	protected ref Widget m_content;
	protected ref ButtonWidget m_submit;
	protected ref ButtonWidget m_new;
	protected ref array<ref FCKV> m_data;
	protected ref array<ref FCAdminListItemForm> m_children;

	protected ref EditBoxWidget m_filterBox;
		
	protected ref FCKV m_new_item;
	
	void FCAdminListForm(Widget parent) {
		int color = ARGB(255, 255, 255, 255);
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("FuelControl/GUI/layouts/fc_admin_list.layout", parent);
		layoutRoot.SetHandler(this);
		m_content = layoutRoot.FindAnyWidget("content");
		m_scroll = layoutRoot.FindAnyWidget("scroll");
		int submit_flags = WidgetFlags.VISIBLE || WidgetFlags.CLIPCHILDREN || WidgetFlags.HEXACTSIZE || WidgetFlags.VEXACTSIZE|| WidgetFlags.HEXACTPOS || WidgetFlags.VEXACTPOS;
		m_submit = ButtonWidget.Cast(layoutRoot.FindAnyWidget("submit"));
		m_new = ButtonWidget.Cast(layoutRoot.FindAnyWidget("new_btn"));
		m_filterBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("filter"));
		m_children = new array<ref FCAdminListItemForm>;
		m_new_item = null;
		m_data = GetData();
		UpdateUI();
	}
	
	void ~FCAdminListForm() {
		delete m_children;
		delete layoutRoot;
	}
	
	array<ref FCKV> GetData() {
		array<ref FCKV> data = new array<ref FCKV>;
		return data;
	}

	void SortKV() {
		bool swapped = true;
		int counter = 0;
		int swaps = 0;
		while (swapped) {          
		     while (counter < m_data.Count() - 1) {
				auto current = m_data.Get(counter);
				auto next = m_data.Get(counter + 1);
		     	if (current.key > next.key) {
					m_data.Set(counter, next);
					m_data.Set(counter + 1, current);
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
	
	array<ref FCKV> FilterKV(array<ref FCKV> input) {
		string filter = m_filterBox.GetText();
		filter.ToLower();
		if (filter != "") {
			auto newArray = new array<ref FCKV>;
			foreach (auto kv: input) {
				string key = kv.key;
				key.ToLower();
				if (key.Contains(filter)) {
					newArray.Insert(kv);
				}
			}
			return newArray;
		} else {
			return input;
		}
	}
	
	

	void UpdateUI() {
		
		for (int i = (m_children.Count() - 1); i >= 0; i--) {
			auto ch = m_children.Get(i);
			m_content.RemoveChild(ch.layoutRoot);
			m_children.Remove(i);
		}

		float root_height;
		float root_width;
		layoutRoot.GetParent().GetScreenSize(root_width, root_height);
		layoutRoot.SetScreenSize(root_width, root_height - 10);
		layoutRoot.SetPos(10, 10);
		m_scroll.SetScreenSize(root_width, root_height - 100);
		m_scroll.SetPos(0, 50);

		float content_height = 0;
		
		SortKV();
		auto data = FilterKV(m_data);
		
		int id = 0;
		if (m_new_item) {
			FCAdminListItemForm new_item = FCAdminListItemForm(m_content, this, id, m_new_item);
			id++;
			new_item.layoutRoot.SetPos(0, 0);
			m_children.Insert(new_item);
			content_height = 30;
		}
		
		foreach (auto fckv: data) {
			FCAdminListItemForm container = new FCAdminListItemForm(m_content, this, id, fckv);
			container.layoutRoot.SetPos(0, content_height);
			m_children.Insert(container);
			content_height = content_height + 30;
			id++;
		}

		m_content.SetScreenSize(root_width, content_height);
		m_content.SetPos(0, 0);
		m_content.Update();
	}
	
	void OnSubmit() {
		delete m_new_item;
		m_filterBox.SetText("");
		m_data = GetData();
		UpdateUI();
	}
	
	void OnDelete(FCAdminListItemForm item) {
		m_content.RemoveChild(item.layoutRoot);
		m_children.Remove(item.m_id);
		if (m_new_item && item.m_id == 0) {
			delete m_new_item;
		} else {
			m_data.Remove(item.m_id);
		}
		UpdateUI();
	}
	
	void OnUpdate(FCAdminListItemForm item) {
		if (m_new_item && item.m_id == 0) {
			m_new_item.key = item.GetKey();
			m_new_item.value = item.GetValue();
		} else {
			auto d = m_data.Get(item.m_id);
			d.key = item.GetKey();
			d.value = item.GetValue();
			m_data.Set(item.m_id, d);
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_submit) {
			OnSubmit();
			return true;
		} else if (w == m_new) {
			m_new_item = new FCKV();
			m_new_item.key = "New_Key";
			m_new_item.value = "New_Value";
			UpdateUI();
			return true;
		}
        return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		if (w == m_filterBox) {
			UpdateUI();
		}
		return false;
	}
}

class FCAdminAutonomyForm: FCAdminListForm {
	override void OnSubmit() {
		FuelControlSettings st = GetFuelControlSettings();
		map<string, float> values = new map<string, float>;
		if (m_new_item) {
			values.Set(m_new_item.key, m_new_item.value.ToFloat());
		}
		foreach (auto kv: m_data) {
			values.Set(kv.key, kv.value.ToFloat());
		}
		st.vehicle_autonomy = values;
		st.SyncSettings(true);
		super.OnSubmit();
	}
	
	override array<ref FCKV> GetData() {
		array<ref FCKV> data = new array<ref FCKV>;
		foreach (auto key, auto value: GetFuelControlSettings().vehicle_autonomy) {
			auto fckv = new FCKV();
			fckv.key = key;
			fckv.value = "" + value;
			data.Insert(fckv);
		}
		return data;
	}
}

class FCAdminTransferRatesForm: FCAdminListForm {
	override void OnSubmit() {
		FuelControlSettings st = GetFuelControlSettings();
		map<string, float> values = new map<string, float>;
		if (m_new_item) {
			values.Set(m_new_item.key, m_new_item.value.ToFloat());
		}
		foreach (auto kv: m_data) {
			values.Set(kv.key, kv.value.ToFloat());
		}
		st.liquid_transfer_rates = values;
		st.SyncSettings(true);
		super.OnSubmit();
	}
	
	override array<ref FCKV> GetData() {
		array<ref FCKV> data = new array<ref FCKV>;
		foreach (auto key, auto value: GetFuelControlSettings().liquid_transfer_rates) {
			auto fckv = new FCKV();
			fckv.key = key;
			fckv.value = "" + value;
			data.Insert(fckv);
		}
		return data;
	}
}