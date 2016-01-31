
#include "ActorPropertySetting.h"

#include "dataset/Actor.h"
#include "view/Context.h"
#include "view/ActorItem.h"
#include "view/ActorsPage.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

ActorPropertySetting::ActorPropertySetting(Actor* actor, 
										   ee::LibraryList* list)
	: IPropertySetting(Context::Instance()->stage->getDesignerPage(), wxT("Actor"))
	, m_actor(actor)
	, m_list(list)
{
}

void ActorPropertySetting::updatePanel(ee::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_actor->name);
		pg->GetProperty(wxT("Tag"))->SetValue(m_actor->tag);
		pg->GetProperty(wxT("X"))->SetValue(m_actor->getPosition().x);
		pg->GetProperty(wxT("Y"))->SetValue(m_actor->getPosition().y);
		pg->GetProperty(wxT("Angle"))->SetValue(m_actor->getAngle());
		pg->GetProperty(wxT("Scale"))->SetValue(m_actor->getScale().x);

		bool xMirror, yMirror;
		m_actor->getMirror(xMirror, yMirror);
		pg->GetProperty(wxT("Horizontal Mirror"))->SetValue(xMirror);
		pg->GetProperty(wxT("Vertical Mirror"))->SetValue(yMirror);

		pg->GetProperty(wxT("As Background"))->SetValue(m_actor->m_isBg);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_actor->name));

		pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, m_actor->tag));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_actor->getPosition().x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_actor->getPosition().y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_actor->getAngle()));
		pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("rad"));
		pg->SetPropertyAttribute(wxT("Angle"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Scale"), wxPG_LABEL, m_actor->getScale().x));
		pg->SetPropertyAttribute(wxT("Scale"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Scale"), "Precision", 2);

		bool xMirror, yMirror;
		m_actor->getMirror(xMirror, yMirror);
		pg->Append(new wxBoolProperty(wxT("Horizontal Mirror"), wxPG_LABEL, xMirror));
		pg->Append(new wxBoolProperty(wxT("Vertical Mirror"), wxPG_LABEL, yMirror));

		pg->Append(new wxBoolProperty(wxT("As Background"), wxPG_LABEL, m_actor->m_isBg));
	}
}

void ActorPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		updateActorName(wxANY_AS(value, wxString));
	else if (name == wxT("Tag"))
		m_actor->tag = wxANY_AS(value, wxString);
	else if (name == wxT("X"))
		m_actor->setTransform(ee::Vector(wxANY_AS(value, float), m_actor->getPosition().y), m_actor->getAngle());
	else if (name == wxT("Y"))
		m_actor->setTransform(ee::Vector(m_actor->getPosition().x, wxANY_AS(value, float)), m_actor->getAngle());
	else if (name == wxT("Angle"))
		m_actor->setTransform(m_actor->getPosition(), wxANY_AS(value, float));
	else if (name == wxT("Scale"))
	{
		float scale = wxANY_AS(value, float);
		m_actor->setScale(scale, scale);
	}
	else if (name == wxT("Horizontal Mirror"))
	{
		bool xMirror, yMirror;
		m_actor->getMirror(xMirror, yMirror);
		m_actor->setMirror(wxANY_AS(value, bool), yMirror);
	}
	else if (name == wxT("Vertical Mirror"))
	{
		bool xMirror, yMirror;
		m_actor->getMirror(xMirror, yMirror);
		m_actor->setMirror(xMirror, wxANY_AS(value, bool));
	}
	else if (name == wxT("As Background"))
		m_actor->m_isBg = wxANY_AS(value, bool);

	Context::Instance()->stage->getDesignerPage()->updateCodePage();

	m_editPanel->Refresh();
}

void ActorPropertySetting::updatePropertyGrid(ee::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void ActorPropertySetting::enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_actor->name));

		pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, m_actor->tag));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_actor->getPosition().x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_actor->getPosition().y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_actor->getAngle()));
		pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("rad"));
		pg->SetPropertyAttribute(wxT("Angle"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Scale"), wxPG_LABEL, m_actor->getScale().x));
		pg->SetPropertyAttribute(wxT("Scale"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Scale"), "Precision", 2);

		bool xMirror, yMirror;
		m_actor->getMirror(xMirror, yMirror);
		pg->Append(new wxBoolProperty(wxT("Horizontal Mirror"), wxPG_LABEL, xMirror));
		pg->Append(new wxBoolProperty(wxT("Vertical Mirror"), wxPG_LABEL, yMirror));

		pg->Append(new wxBoolProperty(wxT("As Background"), wxPG_LABEL, m_actor->m_isBg));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("Tag"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Angle"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale"))->Enable(bEnable);
	pg->GetProperty(wxT("Horizontal Mirror"))->Enable(bEnable);
	pg->GetProperty(wxT("Vertical Mirror"))->Enable(bEnable);
	pg->GetProperty(wxT("As Background"))->Enable(bEnable);
}

void ActorPropertySetting::updateActorName(const wxString& name)
{
	m_actor->name = name;
	m_list->traverse(UpdateNameVisitor(m_actor));
	Context::Instance()->stage->getDesignerPage()->updateCodePage();
	m_list->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class ActorPropertySetting::UpdateNameVisitor
//////////////////////////////////////////////////////////////////////////

void ActorPropertySetting::UpdateNameVisitor::
visit(ee::Object* object, bool& bFetchNext)
{
	ActorItem* item = static_cast<ActorItem*>(object);
	if (item->getActor() == m_actor)
	{
		item->setName(m_actor->name);
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}
