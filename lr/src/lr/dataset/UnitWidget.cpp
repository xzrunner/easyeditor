#include "UnitWidget.h"
#include "UnitInfo.h"

namespace lr
{

//////////////////////////////////////////////////////////////////////////
// class UnitWidget
//////////////////////////////////////////////////////////////////////////

void UnitWidget::LoadFromFile(const Json::Value& value)
{
	m_title = value["title"].asString();
	m_key = value["key"].asString();
	m_special = false;
	if (!value["special"].isNull()) {
		m_special = value["special"].asBool();
	}

	Load(value);
}

//////////////////////////////////////////////////////////////////////////
// class UnitTextWidget
//////////////////////////////////////////////////////////////////////////

std::string UnitTextWidget::GetValue() const
{
	return m_ctrl->GetValue().ToStdString();
}

void UnitTextWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer,
								const UnitInfo& info)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(parent, wxID_ANY, GetTitle()), 0, wxLEFT | wxRIGHT, 5);

	std::string value = info.QueryValue(GetKey());
	if (!value.empty()) {
		m_default = value;
	}
	m_ctrl = new wxTextCtrl(parent, wxID_ANY, m_default, wxDefaultPosition, wxSize(200, -1));
	sizer->Add(m_ctrl, 0, wxLEFT | wxRIGHT, 5);

	top_sizer->Add(sizer);
	top_sizer->AddSpacer(10);
}

void UnitTextWidget::Load(const Json::Value& value)
{
	if (!value["default"].isNull()) {
		m_default = value["default"].asString();
	}
}

//////////////////////////////////////////////////////////////////////////
// class UnitChoiceWidget
//////////////////////////////////////////////////////////////////////////

std::string UnitChoiceWidget::GetValue() const
{
	if (m_ctrl->GetSelection() < m_choices.size()) {
		return m_choices[m_ctrl->GetSelection()].value;
	} else {
		return "";
	}
}

void UnitChoiceWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer,
								  const UnitInfo& info)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(parent, wxID_ANY, GetTitle()), 0, wxLEFT | wxRIGHT, 5);

	std::string value = info.QueryValue(GetKey());

	wxString choices[MAX_ITEMS];
	for (int i = 0, n = m_choices.size(); i < n; ++i) {
		if (m_choices[i].value == value) {
			m_default = i;
		}
		choices[i] = m_choices[i].title;
	}

	m_ctrl = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choices.size(), choices); 
	m_ctrl->SetSelection(m_default);
	sizer->Add(m_ctrl);

	top_sizer->Add(sizer);
	top_sizer->AddSpacer(10);
}

void UnitChoiceWidget::SpecialLoad(const std::string& title, 
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

void UnitChoiceWidget::Load(const Json::Value& value)
{
	m_choices.clear();

	m_default = value["default"].asInt();

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

//////////////////////////////////////////////////////////////////////////
// class UnitComboBoxWidget
//////////////////////////////////////////////////////////////////////////

std::string UnitComboBoxWidget::GetValue() const
{
	return m_ctrl->GetValue().ToStdString();
}

void UnitComboBoxWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer, const UnitInfo& info)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(parent, wxID_ANY, GetTitle()), 0, wxLEFT | wxRIGHT, 5);

	std::string value = info.QueryValue(GetKey());

	wxString choices[MAX_ITEMS];
	for (int i = 0, n = m_choices.size(); i < n; ++i) {
		if (m_choices[i].value == value) {
			m_default = i;
		}
		choices[i] = m_choices[i].title;
	}

	m_ctrl = new wxComboBox(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_choices.size(), choices); 
	m_ctrl->SetSelection(m_default);
	sizer->Add(m_ctrl);

	top_sizer->Add(sizer);
	top_sizer->AddSpacer(10);
}

void UnitComboBoxWidget::Load(const Json::Value& value)
{
	m_choices.clear();

	m_default = value["default"].asInt();

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

//////////////////////////////////////////////////////////////////////////
// class UnitCheckBoxWidget
//////////////////////////////////////////////////////////////////////////

std::string UnitCheckBoxWidget::GetValue() const
{
	if (m_ctrl->IsChecked()) {
		return "true";
	} else {
		return "false";
	}
}

void UnitCheckBoxWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer,
									const UnitInfo& info)
{
	std::string value = info.QueryValue(GetKey());
	if (value == "true") {
		m_default = true;
	} else if (value == "false") {
		m_default = false;
	}

	m_ctrl = new wxCheckBox(parent, wxID_ANY, GetTitle());
	m_ctrl->SetValue(m_default);

	top_sizer->Add(m_ctrl);
	top_sizer->AddSpacer(10);
}

void UnitCheckBoxWidget::Load(const Json::Value& value)
{
	m_default = value["default"].asBool();
}

}