
#include "MovePanel.h"
#include "Move.h"

#include "dataset/Actor.h"
#include "view/Context.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

MovePanel::MovePanel(wxWindow* parent,
					 ebuilder::Move* behavior)
	: BehaviorPanel(parent, MOVE_NAME)
	, m_behavior(behavior)
{
}

wxSizer* MovePanel::createExtend(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_pg = new wxPropertyGrid(extend, wxID_ANY, wxDefaultPosition, wxSize(250, 160), wxPG_SPLITTER_AUTO_CENTER);

	wxPGProperty* startProp = m_pg->Append(new wxStringProperty(wxT("start"),
		wxPG_LABEL, wxT("<composed>")));
	m_pg->AppendIn(startProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, m_behavior->start.x));
	m_pg->SetPropertyAttribute("start.x", wxPG_ATTR_UNITS, wxT("pixels"));
	m_pg->SetPropertyAttribute("start.x", "Precision", 1);
	m_pg->AppendIn(startProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, m_behavior->start.y));
	m_pg->SetPropertyAttribute("start.y", wxPG_ATTR_UNITS, wxT("pixels"));
	m_pg->SetPropertyAttribute("start.y", "Precision", 1);

	wxPGProperty* endProp = m_pg->Append(new wxStringProperty(wxT("end"),
		wxPG_LABEL, wxT("<composed>")));
	m_pg->AppendIn(endProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, m_behavior->end.x));
	m_pg->SetPropertyAttribute("end.x", wxPG_ATTR_UNITS, wxT("pixels"));
	m_pg->SetPropertyAttribute("end.x", "Precision", 1);
	m_pg->AppendIn(endProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, m_behavior->end.y));
	m_pg->SetPropertyAttribute("end.y", wxPG_ATTR_UNITS, wxT("pixels"));
	m_pg->SetPropertyAttribute("end.y", "Precision", 1);

	m_pg->Append(new wxFloatProperty(wxT("speed"), wxPG_LABEL, m_behavior->speed));
	m_pg->SetPropertyAttribute("speed", wxPG_ATTR_UNITS, wxT("pixels/s"));
	m_pg->SetPropertyAttribute("speed", "Precision", 1);

	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(MovePanel::onPGChange));
	sizer->Add(m_pg);

	return sizer;	
}

void MovePanel::onPGChange(wxPropertyGridEvent& event)
{
	m_behavior->start.x = m_pg->GetPropertyValueAsDouble("start.x");
	m_behavior->start.y = m_pg->GetPropertyValueAsDouble("start.y");
	m_behavior->end.x = m_pg->GetPropertyValueAsDouble("end.x");
	m_behavior->end.y = m_pg->GetPropertyValueAsDouble("end.y");
	m_behavior->speed = m_pg->GetPropertyValueAsDouble("speed");

	Actor* actor = m_behavior->getActor();
	actor->setTransform(m_behavior->start, actor->getAngle());

	Context::Instance()->stage->getDesignerPage()->updateCodePage();
}
