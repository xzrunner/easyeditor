#include "SpritePropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/ISprite.h"
#include "view/EditPanel.h"

namespace d2d
{

SpritePropertySetting::SpritePropertySetting(EditPanel* editPanel, ISprite* sprite)
	: IPropertySetting(editPanel, wxT("ISprite"))
	, m_sprite(sprite)
{
}

void SpritePropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_sprite->name);

		pg->GetProperty(wxT("Multi Color"))->SetValue(m_sprite->multiColor);
		pg->GetProperty(wxT("Add Color"))->SetValue(m_sprite->addColor);

 		pg->GetProperty(wxT("X"))->SetValue(m_sprite->getPosition().x);
 		pg->GetProperty(wxT("Y"))->SetValue(m_sprite->getPosition().y);
 		pg->GetProperty(wxT("Angle"))->SetValue(m_sprite->getAngle());
 		pg->GetProperty(wxT("Scale X"))->SetValue(m_sprite->getScaleX());
		pg->GetProperty(wxT("Scale Y"))->SetValue(m_sprite->getScaleY());
		pg->GetProperty(wxT("Shear X"))->SetValue(m_sprite->getShearX());
		pg->GetProperty(wxT("Shear Y"))->SetValue(m_sprite->getShearY());

 		bool xMirror, yMirror;
 		m_sprite->getMirror(xMirror, yMirror);
 		pg->GetProperty(wxT("Horizontal Mirror"))->SetValue(xMirror);
 		pg->GetProperty(wxT("Vertical Mirror"))->SetValue(yMirror);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_sprite->name));

		pg->Append(new wxStringProperty(wxT("Multi Color"), wxPG_LABEL, m_sprite->multiColor));
		pg->Append(new wxStringProperty(wxT("Add Color"), wxPG_LABEL, m_sprite->addColor));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getPosition().x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getPosition().y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_sprite->getAngle()));
		pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("rad"));
		pg->SetPropertyAttribute(wxT("Angle"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Scale X"), wxPG_LABEL, m_sprite->getScaleX()));
		pg->SetPropertyAttribute(wxT("Scale X"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Scale X"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Scale Y"), wxPG_LABEL, m_sprite->getScaleY()));
		pg->SetPropertyAttribute(wxT("Scale Y"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Scale Y"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Shear X"), wxPG_LABEL, m_sprite->getShearX()));
		pg->SetPropertyAttribute(wxT("Shear X"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Shear X"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Shear Y"), wxPG_LABEL, m_sprite->getShearY()));
		pg->SetPropertyAttribute(wxT("Shear Y"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Shear Y"), "Precision", 2);

		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
		pg->Append(new wxBoolProperty(wxT("Horizontal Mirror"), wxPG_LABEL, xMirror));
		pg->Append(new wxBoolProperty(wxT("Vertical Mirror"), wxPG_LABEL, yMirror));
	}
}

void SpritePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_sprite->name = wxANY_AS(value, wxString);
	else if (name == wxT("Multi Color"))
		m_sprite->multiColor = wxANY_AS(value, wxString);
	else if (name == wxT("Add Color"))
		m_sprite->addColor = wxANY_AS(value, wxString);
	else if (name == wxT("X"))
		m_sprite->setTransform(Vector(wxANY_AS(value, float), m_sprite->getPosition().y), m_sprite->getAngle());
	else if (name == wxT("Y"))
		m_sprite->setTransform(Vector(m_sprite->getPosition().x, wxANY_AS(value, float)), m_sprite->getAngle());
	else if (name == wxT("Angle"))
		m_sprite->setTransform(m_sprite->getPosition(), wxANY_AS(value, float));
	else if (name == wxT("Scale X"))
		m_sprite->setScale(wxANY_AS(value, float), m_sprite->getScaleY());
	else if (name == wxT("Scale Y"))
		m_sprite->setScale(m_sprite->getScaleX(), wxANY_AS(value, float));
	else if (name == wxT("Shear X"))
		m_sprite->setShear(wxANY_AS(value, float), m_sprite->getShearY());
	else if (name == wxT("Shear Y"))
		m_sprite->setShear(m_sprite->getShearX(), wxANY_AS(value, float));
	else if (name == wxT("Horizontal Mirror"))
	{
		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
		m_sprite->setMirror(wxANY_AS(value, bool), yMirror);
	}
	else if (name == wxT("Vertical Mirror"))
	{
		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
		m_sprite->setMirror(xMirror, wxANY_AS(value, bool));
	}

	m_editPanel->Refresh();
}

void SpritePropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void SpritePropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_sprite->name));

		pg->Append(new wxStringProperty(wxT("Multi Color"), wxPG_LABEL, m_sprite->multiColor));
		pg->Append(new wxStringProperty(wxT("Add Color"), wxPG_LABEL, m_sprite->addColor));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getPosition().x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getPosition().y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_sprite->getAngle()));
		pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("rad"));
		pg->SetPropertyAttribute(wxT("Angle"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Scale X"), wxPG_LABEL, m_sprite->getScaleX()));
		pg->SetPropertyAttribute(wxT("Scale X"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Scale X"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Scale Y"), wxPG_LABEL, m_sprite->getScaleY()));
		pg->SetPropertyAttribute(wxT("Scale Y"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Scale Y"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Shear X"), wxPG_LABEL, m_sprite->getShearX()));
		pg->SetPropertyAttribute(wxT("Shear X"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Shear X"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("Shear Y"), wxPG_LABEL, m_sprite->getShearY()));
		pg->SetPropertyAttribute(wxT("Shear Y"), wxPG_ATTR_UNITS, wxT("multiple"));
		pg->SetPropertyAttribute(wxT("Shear Y"), "Precision", 2);

		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
		pg->Append(new wxBoolProperty(wxT("Horizontal Mirror"), wxPG_LABEL, xMirror));
		pg->Append(new wxBoolProperty(wxT("Vertical Mirror"), wxPG_LABEL, yMirror));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("Multi Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Add Color"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Angle"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale X"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear X"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Horizontal Mirror"))->Enable(bEnable);
	pg->GetProperty(wxT("Vertical Mirror"))->Enable(bEnable);
}

} // d2d