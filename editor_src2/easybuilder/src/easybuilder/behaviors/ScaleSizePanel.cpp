
#include "ScaleSizePanel.h"
#include "ScaleSize.h"

#include <wx/spinctrl.h>

#include "view/Context.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

ScaleSizePanel::ScaleSizePanel(wxWindow* parent,
							   ScaleSize* behavior)
	: BehaviorPanel(parent, SCALE_SIZE_NAME)
	, m_behavior(behavior)
{
}

wxSizer* ScaleSizePanel::createExtend(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(createSetValueItem(extend));
	return sizer;
}

wxSizer* ScaleSizePanel::createSetValueItem(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(extend, wxID_ANY, wxT("Scale to: ")));

	wxSpinCtrl* scaleCtrl = new wxSpinCtrl(extend, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
		wxSP_ARROW_KEYS, 50, 200, m_behavior->val * 100, wxT("%"));
 	Connect(scaleCtrl->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, 
 		wxSpinEventHandler(ScaleSizePanel::onScaleChanged));
	sizer->Add(scaleCtrl, 0, wxALIGN_LEFT);

	sizer->Add(new wxStaticText(extend, wxID_ANY, wxT("  %")));

	return sizer;
}

void ScaleSizePanel::onScaleChanged(wxSpinEvent& event)
{
	m_behavior->val = event.GetValue() * 0.01f;
	Context::Instance()->stage->getDesignerPage()->updateCodePage();
}
