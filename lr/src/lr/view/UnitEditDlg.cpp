#include "UnitEditDlg.h"

namespace lr
{

static const int MAX_ITEMS = 50;

UnitEditDlg::UnitEditDlg(wxWindow* parent, d2d::ISprite* spr,
						 const std::vector<std::string>& path_names)
	: BaseDialog(parent, wxT("Unit"))
	, m_spr(spr)
	, m_path_names(path_names)
{
	UnitCfg::Instance();

	InitLayout();
}

std::string UnitEditDlg::ToString() const
{
	UnitCfg* cfg = UnitCfg::Instance();

	UnitInfo info(m_spr->tag);

	std::string name = m_name_ctrl->GetValue().ToStdString();
	info.SetValue("name", name);
	m_spr->name = name;

	info.SetValue("camp", cfg->camp_list[m_camp_ctrl->GetSelection()].key);

	info.SetValue("type", cfg->type_list[m_type_ctrl->GetSelection()].key);

	info.SetValue("squad", cfg->squad_list[m_squad_ctrl->GetSelection()].key);

	SetValue(info, "leader", m_leader_ctrl);

	info.SetValue("dir", cfg->dir_list[m_dir_ctrl->GetSelection()].key);

	SetValue(info, "scene_ai", m_scene_ai_ctrl);

	SetValue(info, "god_sight", m_god_sight_ctrl);

	if (m_patrol_ctrl->GetSelection() < m_path_names.size()) {
		info.SetValue("patrol", m_path_names[m_patrol_ctrl->GetSelection()]);
	}

	return info.ToString();
}

void UnitEditDlg::InitLayout()
{
	UnitInfo info(m_spr->tag);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "名字"), 0, wxLEFT | wxRIGHT, 5);

		m_name_ctrl = new wxTextCtrl(this, wxID_ANY, m_spr->name, wxDefaultPosition, wxSize(200, -1));
		sz->Add(m_name_ctrl, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "阵营"), 0, wxLEFT | wxRIGHT, 5);

		m_camp_ctrl = CreateChoiceCtrl(UnitCfg::Instance()->camp_list, info, "camp");
		sz->Add(m_camp_ctrl);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "类型"), 0, wxLEFT | wxRIGHT, 5);

		m_type_ctrl = CreateChoiceCtrl(UnitCfg::Instance()->type_list, info, "type");
		sz->Add(m_type_ctrl);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "队伍"), 0, wxLEFT | wxRIGHT, 5);

		m_squad_ctrl = CreateChoiceCtrl(UnitCfg::Instance()->squad_list, info, "squad");
		sz->Add(m_squad_ctrl);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		m_leader_ctrl = CreateCheckCtrl(info, "leader", "队长");
		sizer->Add(m_leader_ctrl);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "朝向"), 0, wxLEFT | wxRIGHT, 5);

		m_dir_ctrl = CreateChoiceCtrl(UnitCfg::Instance()->dir_list, info, "dir");
		sz->Add(m_dir_ctrl);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		m_scene_ai_ctrl = CreateCheckCtrl(info, "scene_ai", "场景AI");
		sizer->Add(m_scene_ai_ctrl);
	}
	sizer->AddSpacer(10);
	{
		m_god_sight_ctrl = CreateCheckCtrl(info, "god_sight", "上帝视角");
		sizer->Add(m_god_sight_ctrl);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "巡逻"), 0, wxLEFT | wxRIGHT, 5);

		std::vector<std::string> names;
		
		wxString choices[MAX_ITEMS * 10];
		for (int i = 0, n = m_path_names.size(); i < n; ++i) {
			choices[i] = m_path_names[i];
		}
		m_patrol_ctrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_path_names.size(), choices); 
		sz->Add(m_patrol_ctrl);

		std::string key = info.QueryValue("patrol");
		for (int i = 0, n = m_path_names.size(); i < n; ++i) {
			if (key == m_path_names[i]) {
				m_patrol_ctrl->SetSelection(i);
				break;
			}
		}

		sizer->Add(sz);
	}
	sizer->AddSpacer(20);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
		sz->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);
		sizer->Add(sz, 0, wxCENTER);
	}
	sizer->AddSpacer(20);

	SetSizer(sizer);
	sizer->Fit(this);
}

wxChoice* UnitEditDlg::CreateChoiceCtrl(const std::vector<UnitCfg::Item>& items, 
									  const UnitInfo& info, const std::string& key)
{
	wxString choices[MAX_ITEMS];
	for (int i = 0, n = items.size(); i < n; ++i) {
		choices[i] = items[i].title;
	}

	wxChoice* choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, items.size(), choices); 

	std::string val = info.QueryValue(key);
	if (val.empty()) {
		choice->SetSelection(0);
	} else {
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (items[i].key == val) {
				choice->SetSelection(i);
				break;
			}
		}
	}

	return choice;
}

wxCheckBox* UnitEditDlg::CreateCheckCtrl(const UnitInfo& info, const std::string& key,
										 const std::string& title)
{
	wxCheckBox* check = new wxCheckBox(this, wxID_ANY, title);
	check->SetValue(info.QueryValue(key) == "true");
	return check;
}

void UnitEditDlg::SetValue(UnitInfo& info, const std::string& key, wxCheckBox* ctrl)
{
	if (ctrl->IsChecked()) {
		info.SetValue(key, "true");
	} else {
		info.SetValue(key, "false");
	}
}

}