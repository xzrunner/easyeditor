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
	if (choices.empty()) {
		return;
	}

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

void ToolbarPanel::Clear()
{
	m_temp_sizer->Clear(true);
	FillTempSizer();
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
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Ä£°å"));
	m_temp_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
	FillTempSizer();
	return m_temp_sizer;
}

void ToolbarPanel::FillTempSizer()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Ä¿Â¼"));
	wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

	m_temp_dir = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxTE_READONLY);
	sz->Add(m_temp_dir);

	sz->AddSpacer(5);

	wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
	Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetTemplateDir));
	sz->Add(btn);

	m_temp_sizer->Add(sz);
}

void ToolbarPanel::OnChangeAnim(wxCommandEvent& event)
{
	m_ctrl->GetAnimTemplate().SetChoice(event.GetInt());

	try {
		FileIO::Reload(m_ctrl);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
}

void ToolbarPanel::OnSetTemplateDir(wxCommandEvent& event)
{
	d2d::ZoomViewOP* op = static_cast<d2d::ZoomViewOP*>(m_stage->getEditOP());
	op->setMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Template Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		wxString dir = dlg.GetPath();
		m_temp_dir->SetValue(dir);
		m_ctrl->GetAnimTemplate().SetTemplateDir(dir.ToStdString());
	}

	op->setMouseMoveFocus(true);
}

} // eanim