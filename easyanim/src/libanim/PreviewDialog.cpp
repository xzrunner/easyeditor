#include "PreviewDialog.h"
#include "PreviewCanvas.h"
#include "Symbol.h"

#include <ee/EditPanel.h>
#include <ee/ZoomViewOP.h>

#include <wx/wx.h>

namespace eanim
{

PreviewDialog::PreviewDialog(wxWindow* parent, const Symbol* symbol,
							 wxGLContext* glctx)
 	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_symbol(symbol)
{
	InitLayout(glctx);
}

PreviewDialog::~PreviewDialog()
{
//	delete m_stage;
}

void PreviewDialog::InitLayout(wxGLContext* glctx)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
//	buildToolBar(sizer);
	BuildEditPanel(sizer, glctx);
	SetSizer(sizer);
}

void PreviewDialog::BuildToolBar(wxSizer* topSizer)
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

	topSizer->Add(toolSizer, 0);
}

void PreviewDialog::BuildEditPanel(wxSizer* topSizer, wxGLContext* glctx)
{
	m_stage = new ee::EditPanel(this, this);
	m_stage->SetEditOP(new ee::ZoomViewOP(m_stage, m_stage->GetStageImpl(), false));
	m_stage->SetCanvas(new PreviewCanvas(m_stage, m_stage->GetStageImpl(), m_symbol, glctx));
	topSizer->Add(m_stage, 1, wxEXPAND);
}

void PreviewDialog::OnSetCirculate(wxCommandEvent& event)
{
	PreviewCanvas* canvas = static_cast<PreviewCanvas*>(m_stage->GetCanvas());
	assert(canvas);
	PreviewCanvas::PlaySetting& setting = canvas->getPlaySetting();
	setting.isCirculate = event.IsChecked();


	m_stage->SetFocus();
}

void PreviewDialog::OnSetStop(wxCommandEvent& event)
{
	PreviewCanvas* canvas = static_cast<PreviewCanvas*>(m_stage->GetCanvas());
	assert(canvas);
	PreviewCanvas::PlaySetting& setting = canvas->getPlaySetting();
	setting.isStop = event.IsChecked();


	m_stage->SetFocus();
}

} // anim