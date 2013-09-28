#include "PreviewDialog.h"
#include "PreviewCanvas.h"

namespace eanim
{

PreviewDialog::PreviewDialog(wxWindow* parent)
 	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
{
	initLayout();
}

PreviewDialog::~PreviewDialog()
{
//	delete m_stage;
}

void PreviewDialog::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	buildToolBar(sizer);
	buildEditPanel(sizer);
	SetSizer(sizer);
}

void PreviewDialog::buildToolBar(wxSizer* topSizer)
{
	wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);

	wxCheckBox* circulateCheck = new wxCheckBox(this, wxID_ANY, wxT("Ñ­»·"));
	circulateCheck->SetValue(true);
	Connect(circulateCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PreviewDialog::onSetCirculate));
	toolSizer->Add(circulateCheck, 0);

	wxCheckBox* stopCheck = new wxCheckBox(this, wxID_ANY, wxT("ÔÝÍ£"));
	stopCheck->SetValue(false);
	Connect(stopCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PreviewDialog::onSetStop));
	toolSizer->Add(stopCheck, 0);

	topSizer->Add(toolSizer, 0);
}

void PreviewDialog::buildEditPanel(wxSizer* topSizer)
{
	m_stage = new d2d::EditPanel(this, this);
	m_stage->setEditOP(new d2d::ZoomViewOP(m_stage, false));
	m_stage->setCanvas(new PreviewCanvas(m_stage));
	topSizer->Add(m_stage, 1, wxEXPAND);
}

void PreviewDialog::onSetCirculate(wxCommandEvent& event)
{
	PreviewCanvas* canvas = static_cast<PreviewCanvas*>(m_stage->getCanvas());
	assert(canvas);
	PreviewCanvas::PlaySetting& setting = canvas->getPlaySetting();
	setting.isCirculate = event.IsChecked();

	m_stage->Refresh();
	m_stage->SetFocus();
}

void PreviewDialog::onSetStop(wxCommandEvent& event)
{
	PreviewCanvas* canvas = static_cast<PreviewCanvas*>(m_stage->getCanvas());
	assert(canvas);
	PreviewCanvas::PlaySetting& setting = canvas->getPlaySetting();
	setting.isStop = event.IsChecked();

	m_stage->Refresh();
	m_stage->SetFocus();
}

} // eanim