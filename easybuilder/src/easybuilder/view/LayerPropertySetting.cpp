
#include "LayerPropertySetting.h"

#include "dataset/Layer.h"
#include "view/Context.h"
#include "view/LayerItem.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

LayerPropertySetting::LayerPropertySetting(LayerItem* layer, LayersPage* page)
	: IPropertySetting(Context::Instance()->stage->getDesignerPage(), wxT("Layer"))
	, m_layer(layer)
	, m_page(page)
{
}

void LayerPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_layer->getName());

		pg->GetProperty(wxT("Visible"))->SetValue(m_layer->getLayer()->isVisible);

		pg->GetProperty(wxT("Editable"))->SetValue(m_layer->getLayer()->isEditable);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_layer->getName()));

		pg->Append(new wxBoolProperty(wxT("Visible"), wxPG_LABEL, m_layer->getLayer()->isVisible));

		pg->Append(new wxBoolProperty(wxT("Editable"), wxPG_LABEL, m_layer->getLayer()->isEditable));
	}
}

void LayerPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_layer->setName(wxANY_AS(value, wxString));
	else if (name == wxT("Visible"))
		m_layer->getLayer()->isVisible = wxANY_AS(value, bool);
	else if (name == wxT("Editable"))
		m_layer->getLayer()->isEditable = wxANY_AS(value, bool);

	m_editPanel->Refresh();
}

void LayerPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void LayerPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_layer->getName()));

		pg->Append(new wxBoolProperty(wxT("Visible"), wxPG_LABEL, m_layer->getLayer()->isVisible));

		pg->Append(new wxBoolProperty(wxT("Editable"), wxPG_LABEL, m_layer->getLayer()->isEditable));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("Visible"))->Enable(bEnable);
	pg->GetProperty(wxT("Editable"))->Enable(bEnable);
}