#include "PreviewDialog.h"
#include "PreviewPanel.h"
#include "PreviewCanvas.h"
#include "PreviewOP.h"

#include <ee/Config.h>
#include <ee/SettingData.h>

#include <sprite2/AnimCurr.h>
#include <gum/Audio.h>

namespace eanim
{

PreviewDialog::PreviewDialog(wxWindow* parent, wxGLContext* glctx, s2::AnimSymbol* sym)
 	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
{
	gum::Audio::Instance()->Stop();

	sym->LoadCopy();
	m_curr.SetAnimCopy(dynamic_cast<s2::AnimSymbol*>(sym)->GetCopy());

	InitLayout(glctx);

	const pt2::Color& col = ee::Config::Instance()->GetSettings().bg_color;
	m_stage->GetCanvas()->SetBgColor(col);
}

PreviewDialog::~PreviewDialog()
{
	gum::Audio::Instance()->Stop();

//	delete m_stage;
}

void PreviewDialog::InitLayout(wxGLContext* glctx)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	BuildToolBar(sizer);
	BuildEditPanel(sizer, glctx);
	SetSizer(sizer);
}

void PreviewDialog::BuildToolBar(wxSizer* top_sizer)
{
	wxBoxSizer* tool_sizer = new wxBoxSizer(wxVERTICAL);

	wxCheckBox* loop_check = new wxCheckBox(this, wxID_ANY, wxT("Ñ­»·"));
	loop_check->SetValue(true);
	Connect(loop_check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PreviewDialog::OnSetLoop));
	tool_sizer->Add(loop_check, 0);

	top_sizer->Add(tool_sizer, 0);
}

void PreviewDialog::BuildEditPanel(wxSizer* top_sizer, wxGLContext* glctx)
{
	m_stage = new PreviewPanel(this, this, m_curr);
	m_stage->SetEditOP(std::make_shared<PreviewOP>(m_stage, m_stage->GetStageImpl(), m_curr));
	m_stage->SetCanvas(std::make_shared<PreviewCanvas>(m_stage, m_stage->GetStageImpl(), glctx, m_curr));
	top_sizer->Add(m_stage, 1, wxEXPAND);
}

void PreviewDialog::OnSetLoop(wxCommandEvent& event)
{
	m_stage->SetLoop(event.IsChecked());
	m_stage->SetFocus();
}

} // eanim