
#include "BehaviorPanel.h"

#include <wx/statline.h>

using namespace ebuilder;

const wxString BehaviorPanel::TIP_DRAG_BEHAVIOR = wxT("drag in a behavior");

const wxSize BehaviorPanel::TIP_SIZE = wxSize(200, -1);

BehaviorPanel::BehaviorPanel(wxWindow* parent, const wxString& name)
	: CollapsiblePane(parent, name)
{
}

void BehaviorPanel::createPanel(wxWindow* parent)
{
	wxWindow* extend = GetPane();
	wxSizer* sizer = createExtend(extend);
	extend->SetSizer(sizer);

	Collapse(false);

	//	parent->GetSizer()->Add(new wxStaticLine(parent), 0, wxEXPAND);
	parent->GetSizer()->Add(this, 0, /*wxEXPAND*/wxALL/*wxALIGN_CENTER*/, 10);
	//	parent->GetSizer()->Add(new wxStaticLine(parent), 0, wxEXPAND);
	//	parent->GetSizer()->AddSpacer(10);

	parent->Layout();
	parent->Refresh();

	//////////////////////////////////////////////////////////////////////////

//	CollapsiblePane* panel = new CollapsiblePane(parent, m_name);
//	wxWindow* extend = panel->GetPane();
//	wxSizer* sizer = createExtend(extend);
//	extend->SetSizer(sizer);
//	panel->Collapse(false);
//
////	parent->GetSizer()->Add(new wxStaticLine(parent), 0, wxEXPAND);
//	parent->GetSizer()->Add(panel, 0, /*wxEXPAND*/wxALL/*wxALIGN_CENTER*/, 10);
////	parent->GetSizer()->Add(new wxStaticLine(parent), 0, wxEXPAND);
////	parent->GetSizer()->AddSpacer(10);
//
//	parent->Layout();
//	parent->Refresh();
}
