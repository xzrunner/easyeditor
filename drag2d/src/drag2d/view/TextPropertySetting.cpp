#include "TextPropertySetting.h"
#include "PropertySettingPanel.h"

#include <wx/propgrid/advprops.h>

#include "dataset/TextSprite.h"
#include "view/EditPanel.h"

namespace d2d
{

TextPropertySetting::TextPropertySetting(EditPanel* stage, TextSprite* sprite)
	: IPropertySetting("TextSprite")
	, m_stage(stage)
	, m_sprite(sprite)
{
}

void TextPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == "Text")
	{
		wxString text = wxANY_AS(value, wxString);
		m_sprite->setText(text.ToStdString());
	}
	else if (name == "Size")
	{
		m_sprite->setSize(wxANY_AS(value, float));
	}
	else if (name == "Color")
	{
		wxColour c = wxANY_AS(value, wxColour);
		m_sprite->setColor(d2d::Colori(c.Red(), c.Green(), c.Blue(), c.Alpha()));
	}

	m_stage->RefreshStage();
}

void TextPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Text"))->SetValue(m_sprite->getText());

	pg->GetProperty(wxT("Size"))->SetValue(m_sprite->getSize());

	const d2d::Colori& c = m_sprite->getColor();
	wxString str = "("+wxString::FromDouble(c.r)+","+wxString::FromDouble(c.g)+","+wxString::FromDouble(c.b)+","+wxString::FromDouble(c.a)+")";
	pg->SetPropertyValueString(wxT("Color"), str);
}

void TextPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Text"), wxPG_LABEL, m_sprite->getText()));

	pg->Append(new wxFloatProperty(wxT("Size"), wxPG_LABEL, m_sprite->getSize()));
	pg->SetPropertyAttribute(wxT("Size"), "Precision", 0);

	const d2d::Colori& c = m_sprite->getColor();
	pg->Append(new wxColourProperty("Color", wxPG_LABEL, wxColour(c.r, c.g, c.b, c.a)));
	pg->SetPropertyAttribute("Color", "HasAlpha", false);
}

} // d2d