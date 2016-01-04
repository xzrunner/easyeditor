#include "PreviewDialog.h"
#include "PreviewPanel.h"
#include "PreviewCanvas.h"
#include "PreviewOP.h"

#include "message/messages.h"

namespace eanim
{

PreviewDialog::PreviewDialog(wxWindow* parent, wxGLContext* glctx)
 	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_control(0.033f)
{
	int fps = GetFpsSJ::Instance()->Get();
	m_control.SetDt(1.0f / fps);
	InitLayout(glctx);

	const d2d::Colorf& col = d2d::Config::Instance()->GetSettings().bg_color;
	m_stage->GetCanvas()->SetBgColor(col);
}

PreviewDialog::~PreviewDialog()
{
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
	wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);

	wxCheckBox* circulateCheck = new wxCheckBox(this, wxID_ANY, wxT("Ñ­»·"));
	circulateCheck->SetValue(true);
	Connect(circulateCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PreviewDialog::OnSetCirculate));
	toolSizer->Add(circulateCheck, 0);

	wxCheckBox* stopCheck = new wxCheckBox(this, wxID_ANY, wxT("ÔÝÍ£"));
	stopCheck->SetValue(false);
	Connect(stopCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PreviewDialog::OnSetStop));
	toolSizer->Add(stopCheck, 0);

	top_sizer->Add(toolSizer, 0);
}

void PreviewDialog::BuildEditPanel(wxSizer* top_sizer, wxGLContext* glctx)
{
	m_stage = new PreviewPanel(this, this, m_control);
	m_stage->SetEditOP(new PreviewOP(m_stage, m_stage->GetStageImpl(), m_settings, m_control));
	m_stage->SetCanvas(new PreviewCanvas(m_stage, m_stage->GetStageImpl(), m_settings, m_control, glctx));
	top_sizer->Add(m_stage, 1, wxEXPAND);
}

void PreviewDialog::OnSetCirculate(wxCommandEvent& event)
{
	m_settings.isCirculate = event.IsChecked();
	m_stage->SetFocus();
}

void PreviewDialog::OnSetStop(wxCommandEvent& event)
{
	m_settings.isStop = event.IsChecked();
	m_stage->SetFocus();
}

} // eanim