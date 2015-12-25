#include "ChoiceWidget.h"
#include "DynamicInfo.h"

namespace lr
{

std::string ChoiceWidget::GetValue() const
{
	int sel = m_ctrl->GetSelection();
	if (sel == m_default) {
		return DEFAULT_VAL;
	} else {
		if (sel < m_choices.size()) {
			return m_choices[sel].value;
		} else {
			return "";
		}
	}
}

void ChoiceWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer,
								  const DynamicInfo& info)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(parent, wxID_ANY, GetTitle()), 0, wxLEFT | wxRIGHT, 5);

	int default_sel = m_default;
	std::string value = info.QueryValue(GetKey());
	wxString choices[MAX_ITEMS];
	for (int i = 0, n = m_choices.size(); i < n; ++i) {
		if (m_choices[i].value == value) {
			default_sel = i;
		}
		choices[i] = m_choices[i].title;
	}

	m_ctrl = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choices.size(), choices); 
	m_ctrl->SetSelection(default_sel);
	sizer->Add(m_ctrl);

	top_sizer->Add(sizer);
	top_sizer->AddSpacer(10);
}

void ChoiceWidget::SpecialLoad(const std::string& title, 
								   const std::string& key,
								   const std::vector<std::string>& items, 
								   const std::string& default_item)
{
	m_title = title;
	m_key = key;

	m_choices.clear();
	for (int i = 0, n = items.size(); i < n; ++i) {
		if (default_item == items[i]) {
			m_default = i;
		}

		Item item;
		item.value = items[i];
		item.title = items[i];
		m_choices.push_back(item);
	}
}

void ChoiceWidget::Load(const Json::Value& value)
{
	m_choices.clear();

	if (!value["default"].isNull()) {
		m_default = value["default"].asInt();		
	} else {
		m_default = -1;
	}

	int idx = 0;
	Json::Value v = value["choices"][idx++];
	while (!v.isNull()) {
		Item item;
		item.value = v["value"].asString();
		item.title = v["title"].asString();
		m_choices.push_back(item);

		v = value["choices"][idx++];
	}
}

}