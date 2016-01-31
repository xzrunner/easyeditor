#include "ToolbarPanel.h"
#include "StagePanel.h"

#include "edit/CommonCMPT.h"
#include "edit/SkeletonCMPT.h"
#include "view/ViewMgr.h"
#include "dataset/DataMgr.h"
#include "frame/FileIO.h"

namespace eanim
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, bool vertical)
	: ee::ToolbarPanel(parent, ViewMgr::Instance()->stage->GetStageImpl(), vertical)
	, m_tl_anim_choice(NULL)
{
	addChild(new CommonCMPT(this, "Common", vertical));
	addChild(new SkeletonCMPT(this, "Skeleton", vertical));
	SetSizer(InitLayout());	

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

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	{
		sizer->Add(InitChildrenLayout());
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
	DataMgr::Instance()->GetTemplate().SetChoice(event.GetInt());

	try {
		ee::ClearSpriteSJ::Instance()->Clear();
		FileIO::Reload();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
}

void ToolbarPanel::OnSetTemplateDir(wxCommandEvent& event)
{
	ee::ZoomViewOP* op = static_cast<ee::ZoomViewOP*>(m_stage->GetEditOP());
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Template Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		wxString dir = dlg.GetPath();
		m_tl_dir_text->SetValue(dir);
		DataMgr::Instance()->GetTemplate().SetTemplateDir(dir.ToStdString());
	}

	op->SetMouseMoveFocus(true);
}

} // eanim