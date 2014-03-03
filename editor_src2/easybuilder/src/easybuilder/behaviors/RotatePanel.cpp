
#include "RotatePanel.h"
#include "Rotate.h"

#include "dataset/Actor.h"
#include "view/Context.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

RotatePanel::RotatePanel(wxWindow* parent, ebuilder::Rotate* behavior)
	: BehaviorPanel(parent, ROTATE_NAME)
	, m_behavior(behavior)
{
}

wxSizer* RotatePanel::createExtend(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_pg = new wxPropertyGrid(extend, wxID_ANY, wxDefaultPosition, wxSize(250, 100), wxPG_SPLITTER_AUTO_CENTER);

	m_pg->Append(new wxFloatProperty(wxT("start"), wxPG_LABEL, m_behavior->start));
	m_pg->SetPropertyAttribute("start", wxPG_ATTR_UNITS, wxT("arc"));
	m_pg->SetPropertyAttribute("start", "Precision", 1);

	m_pg->Append(new wxFloatProperty(wxT("end"), wxPG_LABEL, m_behavior->end));
	m_pg->SetPropertyAttribute("end", wxPG_ATTR_UNITS, wxT("arc"));
	m_pg->SetPropertyAttribute("end", "Precision", 1);

	m_pg->Append(new wxFloatProperty(wxT("speed"), wxPG_LABEL, m_behavior->speed));
	m_pg->SetPropertyAttribute("speed", wxPG_ATTR_UNITS, wxT("arc/s"));
	m_pg->SetPropertyAttribute("speed", "Precision", 1);

	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(RotatePanel::onPGChange));
	sizer->Add(m_pg);

	return sizer;	
}

void RotatePanel::onPGChange(wxPropertyGridEvent& event)
{
	m_behavior->start = m_pg->GetPropertyValueAsDouble("start");
	m_behavior->end = m_pg->GetPropertyValueAsDouble("end");
	m_behavior->speed = m_pg->GetPropertyValueAsDouble("speed");

	Actor* actor = m_behavior->getActor();
	actor->setTransform(actor->getPosition(), m_behavior->start);

	Context::Instance()->stage->getDesignerPage()->updateCodePage();
}
