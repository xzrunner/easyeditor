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
						   bool vertical, Controller* ctrl)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl(), vertical)
	, m_ctrl(ctrl)
	, m_tl_anim_choice(NULL)
{
	addChild(new CommonCMPT(this, "Common", stage, property, vertical, ctrl));
	addChild(new SkeletonCMPT(this, "Skeleton", stage, property, vertical));
	SetSizer(initLayout());	

	ChangeTemplateMode(true);
}

void ToolbarPanel::AddAnimChoice(const std::vector<std::string>& choices)
{
	if (choices.empty()) {
		return;
	}

	m_tl_anim_choice->Clear();
	for (int i = 0, n = choices.size(); i < n; ++i) {
		m_tl_anim_choice->Append(choices[i]);
	}
	m_tl_anim_choice->SetSelection(0);
}

void ToolbarPanel::ChangeTemplateMode(bool single)
{
	m_tl_sizer->Show(m_tl_single_sizer, single, true);
	m_tl_sizer->Show(m_tl_multi_sizer, !single, true);
	m_tl_sizer->Layout();
}

void ToolbarPanel::Clear()
{
	ChangeTemplateMode(true);
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
 	m_tl_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Ä¿Â¼"));
 		m_tl_single_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
 
 		m_tl_dir_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxTE_READONLY);
 		m_tl_single_sizer->Add(m_tl_dir_text);
 
 		m_tl_single_sizer->AddSpacer(5);
 
 		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
 		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetTemplateDir));
 		m_tl_single_sizer->Add(btn);
	}
	m_tl_sizer->Add(m_tl_single_sizer);
	{
		m_tl_multi_sizer = new wxBoxSizer(wxVERTICAL);

		m_tl_anim_choice = new wxChoice(this, wxID_ANY);
		Connect(m_tl_anim_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ToolbarPanel::OnChangeAnim));
		m_tl_multi_sizer->Add(m_tl_anim_choice);
	}
	m_tl_sizer->Add(m_tl_multi_sizer);
	return m_tl_sizer;
}

void ToolbarPanel::OnChangeAnim(wxCommandEvent& event)
{
	m_ctrl->GetAnimTemplate().SetChoice(event.GetInt());

	try {
		d2d::ClearSpriteSJ::Instance()->Clear();
		FileIO::Reload(m_ctrl);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
}

void ToolbarPanel::OnSetTemplateDir(wxCommandEvent& event)
{
	d2d::ZoomViewOP* op = static_cast<d2d::ZoomViewOP*>(m_stage->GetEditOP());
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Template Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		wxString dir = dlg.GetPath();
		m_tl_dir_text->SetValue(dir);
		m_ctrl->GetAnimTemplate().SetTemplateDir(dir.ToStdString());
	}

	op->SetMouseMoveFocus(true);
}

} // eanim