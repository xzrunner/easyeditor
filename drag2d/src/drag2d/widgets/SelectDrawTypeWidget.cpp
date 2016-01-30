#include "SelectDrawTypeWidget.h"

#include "view/OrthoCanvas.h"
#include "common/Settings.h"

namespace d2d
{

SelectDrawTypeWidget::SelectDrawTypeWidget(wxWindow* parent, OrthoCanvas* canvas)
	: AbstractWidget(parent)
	, m_canvas(canvas)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("物理+图片"));
	choices.Add(wxT("物理"));
	choices.Add(wxT("图片"));

	wxRadioBox* drawChoice = new wxRadioBox(this, wxID_ANY, wxT("显示方式"), 
		wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(drawChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
		wxCommandEventHandler(SelectDrawTypeWidget::onSetDrawType));
	drawChoice->SetSelection(Settings::draw_type);

	sizer->Add(drawChoice);

	SetSizer(sizer);
}

void SelectDrawTypeWidget::onSetDrawType(wxCommandEvent& event)
{
	Settings::draw_type = static_cast<Settings::DrawType>(event.GetSelection());
	m_canvas->Refresh(true);
}

}