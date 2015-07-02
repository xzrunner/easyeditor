#include "ToolbarPanel.h"
#include "StagePanel.h"

#include "edit/CommonCMPT.h"
#include "edit/SkeletonCMPT.h"
#include "frame/FileIO.h"
#include "frame/Controller.h"

namespace eanim
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, 
						   d2d::PropertySettingPanel* property, 
						   d2d::ViewPanelMgr* view_panel_mgr,
						   bool vertical, Controller* ctrl)
	: d2d::ToolbarPanel(parent, stage, vertical)
	, m_ctrl(ctrl)
	, m_anim_choice(NULL)
{
	addChild(new CommonCMPT(this, wxT("Common"), stage, property, view_panel_mgr, vertical, ctrl));
	addChild(new SkeletonCMPT(this, wxT("Skeleton"), stage, property, vertical));
	SetSizer(initLayout());	
}

void ToolbarPanel::AddAnimChoice(const std::vector<std::string>& choices)
{
	m_temp_open->SetValue(true);

	if (m_anim_choice) 
		delete m_anim_choice;

	wxArrayString array;
	for (size_t i = 0, n = choices.size(); i < n; ++i)
		array.push_back(choices[i]);

	m_anim_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, array);
	m_anim_choice->SetSelection(0);
	Connect(m_anim_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ToolbarPanel::OnChangeAnim));
	m_temp_sizer->Clear(true);
	m_temp_sizer->Add(m_anim_choice);

	Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	{
		sizer->Add(initChildrenLayout());
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = InitTemplateLayout();
		sizer->Add(sz);
	}
	return sizer;
}

wxSizer* ToolbarPanel::InitTemplateLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("模板"));
	m_temp_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

	m_temp_open = new wxCheckBox(this, wxID_ANY, "保存模板信息");
	m_temp_open->SetValue(false);
	Connect(m_temp_open->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxCommandEventHandler(ToolbarPanel::OnOpenTemplate));
	m_temp_sizer->Add(m_temp_open);

	return m_temp_sizer;
}

void ToolbarPanel::OnChangeAnim(wxCommandEvent& event)
{
	m_ctrl->GetAnimTemplate().SetChoice(event.GetInt());

	try {
		FileIO::reload(m_ctrl);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
}

void ToolbarPanel::OnOpenTemplate(wxCommandEvent& event)
{
	if (event.IsChecked()) {
		// todo m_ctrl->GetAnimTemplate().Add()
		//
	} else {
		m_ctrl->GetAnimTemplate().Clear();
	}
}

} // eanim