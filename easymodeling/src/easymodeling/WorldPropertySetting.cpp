#include "WorldPropertySetting.h"
#include "Context.h"

#include <easymodeling.h>

namespace emodeling
{

WorldPropertySetting::WorldPropertySetting(d2d::EditPanelImpl* stage)
	: d2d::IPropertySetting("World")
	, m_stage(stage)
{
}

void WorldPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	libmodeling::World* world = Context::Instance()->world;

	bool dirty = true;
	if (name == wxT("Gravity X"))
		world->gravity.x = wxANY_AS(value, float);
	else if (name == wxT("Gravity Y"))
		world->gravity.y = wxANY_AS(value, float);
	else if (name == wxT("AllowSleep"))
		world->allowSleep = wxANY_AS(value, bool);
	else if (name == wxT("WarmStarting"))
		world->warmStarting = wxANY_AS(value, bool);
	else if (name == wxT("ContinuousPhysics"))
		world->continuousPhysics = wxANY_AS(value, bool);
	else if (name == wxT("SubStepping"))
		world->subStepping = wxANY_AS(value, bool);
	else if (name == wxT("VelocityIterations"))
		world->velocityIterations = wxANY_AS(value, float);
	else if (name == wxT("PositionIterations"))
		world->positionIterations = wxANY_AS(value, float);
	else
		dirty = false;

	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

void WorldPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	const libmodeling::World* world = Context::Instance()->world;

	pg->GetProperty(wxT("Gravity X"))->SetValue(world->gravity.x);
	pg->GetProperty(wxT("Gravity Y"))->SetValue(world->gravity.y);

	pg->GetProperty(wxT("AllowSleep"))->SetValue(world->allowSleep);
	pg->GetProperty(wxT("WarmStarting"))->SetValue(world->warmStarting);
	pg->GetProperty(wxT("ContinuousPhysics"))->SetValue(world->continuousPhysics);
	pg->GetProperty(wxT("SubStepping"))->SetValue(world->subStepping);

	pg->GetProperty(wxT("VelocityIterations"))->SetValue(world->velocityIterations);
	pg->GetProperty(wxT("PositionIterations"))->SetValue(world->positionIterations);
}

void WorldPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	const libmodeling::World* world = Context::Instance()->world;

	pg->Clear();

	pg->Append(new wxFloatProperty(wxT("Gravity X"), wxPG_LABEL, world->gravity.x));
	pg->Append(new wxFloatProperty(wxT("Gravity Y"), wxPG_LABEL, world->gravity.y));

	pg->Append(new wxBoolProperty(wxT("AllowSleep"), wxPG_LABEL, world->allowSleep));
	pg->SetPropertyAttribute("AllowSleep", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("WarmStarting"), wxPG_LABEL, world->warmStarting));
	pg->SetPropertyAttribute("WarmStarting", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("ContinuousPhysics"), wxPG_LABEL, world->continuousPhysics));
	pg->SetPropertyAttribute("ContinuousPhysics", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("SubStepping"), wxPG_LABEL, world->subStepping));
	pg->SetPropertyAttribute("SubStepping", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxIntProperty(wxT("VelocityIterations"), wxPG_LABEL, world->velocityIterations));
	pg->Append(new wxIntProperty(wxT("PositionIterations"), wxPG_LABEL, world->positionIterations));
}

}