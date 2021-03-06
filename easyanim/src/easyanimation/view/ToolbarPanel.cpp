#include "ToolbarPanel.h"
#include "StagePanel.h"

#include "edit/NormalCMPT.h"
#include "edit/SkeletonCMPT.h"
#include "view/ViewMgr.h"
#include "dataset/DataMgr.h"
#include "dataset/Layer.h"
#include "frame/FileIO.h"

#include <ee/sprite_msg.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/ZoomViewOP.h>
#include <ee/EditPanelImpl.h>

namespace eanim
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, bool vertical)
	: ee::ToolbarPanel(parent, ViewMgr::Instance()->stage->GetStageImpl(), vertical)
	, m_tl_anim_choice(NULL)
{
	AddChild(new NormalCMPT(this, "Normal", vertical));
	AddChild(new SkeletonCMPT(this, "Skeleton", vertical));
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
		sizer->Add(InitTemplateLayout());
	}
	sizer->AddSpacer(10);
	{
		sizer->Add(InitCommonLayout());
	}
	return sizer;
}

wxSizer* ToolbarPanel::InitTemplateLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("ģ��"));
 	m_tl_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Ŀ¼"));
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

wxSizer* ToolbarPanel::InitCommonLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Common");
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	
	wxCheckBox* op_pass = new wxCheckBox(this, wxID_ANY, "�����̳�");
	op_pass->SetValue(false);
	Connect(op_pass->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnOpPass));
	sizer->Add(op_pass);
	
	return sizer;
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
	auto op = std::dynamic_pointer_cast<ee::ZoomViewOP>(m_stage->GetEditOP());
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Template Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		std::string dir = dlg.GetPath();
		m_tl_dir_text->SetValue(dir);
		DataMgr::Instance()->GetTemplate().SetTemplateDir(dir);
	}

	op->SetMouseMoveFocus(true);
}

void ToolbarPanel::OnOpPass(wxCommandEvent& event)
{
	bool enable = event.IsChecked();
	const std::vector<Layer*>& layers = DataMgr::Instance()->GetLayers().GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		layers[i]->GetSpriteObserver().Enable(enable);
	}
}

} // eanim