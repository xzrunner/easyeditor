#include "PreviewDialog.h"
#include "PreviewCanvas.h"
#include "PreviewOP.h"

#include "frame/Controller.h"

namespace eanim
{

PreviewDialog::PreviewDialog(wxWindow* parent, Controller* ctrl)
 	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_control(0.033f)
	, m_ctrl(ctrl)
{
	m_control.SetDt(1.0f / ctrl->fps);
	initLayout();

	const d2d::Colorf& col = d2d::Config::Instance()->GetSettings().bg_color;
	m_stage->getCanvas()->setBgColor(col);
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
	m_stage->setEditOP(new PreviewOP(m_stage, m_settings, m_control));
	m_stage->setCanvas(new PreviewCanvas(m_stage, m_settings, m_control, m_ctrl));
	topSizer->Add(m_stage, 1, wxEXPAND);
}

void PreviewDialog::onSetCirculate(wxCommandEvent& event)
{
	m_settings.isCirculate = event.IsChecked();

	m_stage->Refresh();
	m_stage->SetFocus();
}

void PreviewDialog::onSetStop(wxCommandEvent& event)
{
	m_settings.isStop = event.IsChecked();

	m_stage->Refresh();
	m_stage->SetFocus();
}

} // eanim