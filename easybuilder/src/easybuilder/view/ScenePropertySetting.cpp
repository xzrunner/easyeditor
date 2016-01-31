
#include "ScenePropertySetting.h"
#include "Context.h"
#include "StagePanel.h"
#include "SceneItem.h"
#include "ScenesPage.h"
#include "ListObserver.h"

#include "output/DesignerPage.h"
#include "dataset/Scene.h"

#include <wx/propgrid/advprops.h>

using namespace ebuilder;

ScenePropertySetting::ScenePropertySetting(SceneItem* scene, ScenesPage* page)
	: IPropertySetting(Context::Instance()->stage->getDesignerPage(), wxT("Scene"))
	, m_scene(scene)
	, m_page(page)
{
}

void ScenePropertySetting::updatePanel(ee::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_scene->getName());

		pg->GetProperty(wxT("Tag"))->SetValue(m_scene->getScene()->m_tag);

		wxString str = m_scene->getScene()->m_color.GetAsString(wxC2S_HTML_SYNTAX);
		pg->GetProperty(wxT("Background Color"))->SetValueFromString(str);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_scene->getName()));

		pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, m_scene->getScene()->m_tag));

		pg->Append(new wxColourProperty(wxT("Background Color"), wxPG_LABEL, *wxWHITE));
	}
}

void ScenePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		updateSceneName(wxANY_AS(value, wxString));
	else if (name == wxT("Tag"))
		m_scene->getScene()->m_tag = wxANY_AS(value, wxString);
	else if (name == wxT("Background Color"))
		m_scene->getScene()->m_color = (wxANY_AS(value, wxColour));
}

void ScenePropertySetting::updatePropertyGrid(ee::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void ScenePropertySetting::enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable)
{
}

void ScenePropertySetting::updateSceneName(const wxString& name)
{
	m_scene->setName(name);
	m_page->traverse(UpdateNameVisitor(m_scene));
	m_page->getListObserver()->updateListeners();
	Context::Instance()->stage->updateCodePages(m_scene);
	m_page->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class ScenePropertySetting::UpdateNameVisitor
//////////////////////////////////////////////////////////////////////////

void ScenePropertySetting::UpdateNameVisitor::
visit(ee::Object* object, bool& bFetchNext)
{
	SceneItem* item = static_cast<SceneItem*>(object);
	if (item == m_scene)
	{
		item->setName(m_scene->getName());
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}
