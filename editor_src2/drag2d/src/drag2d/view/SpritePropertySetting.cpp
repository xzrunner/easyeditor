#include "SpritePropertySetting.h"
#include "PropertySettingPanel.h"

#include "history/ArrangeSpriteAtomicOP.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "common/Math.h"
#include "common/color_trans.h"

#include <wx/propgrid/advprops.h>

namespace d2d
{

SpritePropertySetting::SpritePropertySetting(EditPanel* editPanel, ISprite* sprite)
	: IPropertySetting(editPanel, wxT("Sprite"))
	, m_sprite(sprite)
{
}

void SpritePropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();
	if (getPGType(pg) == m_type)
		updateProperties(pg);
	else
		initProperties(pg);
}

void SpritePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_sprite->name = wxANY_AS(value, wxString);
	else if (name == wxT("Multi Color")) {
		wxColour col = wxANY_AS(value, wxColour);
		m_sprite->multiCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == wxT("Add Color")) {
		wxColour col = wxANY_AS(value, wxColour);
		m_sprite->addCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == wxT("X"))
		translate(wxANY_AS(value, float), m_sprite->getPosition().y);
	else if (name == wxT("Y"))
		translate(m_sprite->getPosition().x, wxANY_AS(value, float));
	else if (name == wxT("Angle"))
		rotate(wxANY_AS(value, float) * TRANS_DEG_TO_RAD);
	else if (name == wxT("Scale X"))
		scale(wxANY_AS(value, float), m_sprite->getScaleY());
	else if (name == wxT("Scale Y"))
		scale(m_sprite->getScaleX(), wxANY_AS(value, float));
	else if (name == wxT("Width"))
	{
		const float width = wxANY_AS(value, float);
		scale(width/m_sprite->getSymbol().getWidth(), m_sprite->getScaleY());
	}
	else if (name == wxT("Height"))
	{
		const float height = wxANY_AS(value, float);
		scale(m_sprite->getScaleX(), height/m_sprite->getSymbol().getHeight());
	}
	else if (name == wxT("Shear X"))
		shear(wxANY_AS(value, float), m_sprite->getShearY());
	else if (name == wxT("Shear Y"))
		shear(m_sprite->getShearX(), wxANY_AS(value, float));
	else if (name == wxT("Horizontal Mirror"))
	{
		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
//		m_sprite->setMirror(wxANY_AS(value, bool), yMirror);
		mirror(wxANY_AS(value, bool), yMirror);
	}
	else if (name == wxT("Vertical Mirror"))
	{
		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
//		m_sprite->setMirror(xMirror, wxANY_AS(value, bool));
		mirror(xMirror, wxANY_AS(value, bool));
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
		initProperties(pg);

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("Multi Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Add Color"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Angle"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale X"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Width"))->Enable(bEnable);
	pg->GetProperty(wxT("Height"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear X"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Horizontal Mirror"))->Enable(bEnable);
	pg->GetProperty(wxT("Vertical Mirror"))->Enable(bEnable);
}

void SpritePropertySetting::updateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_sprite->name);

// 	pg->GetProperty(wxT("Multi Color"))->SetValue(m_sprite->multiColor);
// 	pg->GetProperty(wxT("Add Color"))->SetValue(m_sprite->addColor);

	wxColour mul_col = wxColour(m_sprite->multiCol.r*255, m_sprite->multiCol.g*255, m_sprite->multiCol.b*255, m_sprite->multiCol.a*255);
	wxColour add_col = wxColour(m_sprite->addCol.r*255, m_sprite->addCol.g*255, m_sprite->addCol.b*255, m_sprite->addCol.a*255);
	pg->SetPropertyValueString(wxT("Multi Color"), mul_col.GetAsString());
	pg->SetPropertyValueString(wxT("Add Color"), add_col.GetAsString());

	pg->GetProperty(wxT("X"))->SetValue(m_sprite->getPosition().x);
	pg->GetProperty(wxT("Y"))->SetValue(m_sprite->getPosition().y);
	pg->GetProperty(wxT("Angle"))->SetValue(m_sprite->getAngle() * TRANS_RAD_TO_DEG);
	pg->GetProperty(wxT("Scale X"))->SetValue(m_sprite->getScaleX());
	pg->GetProperty(wxT("Scale Y"))->SetValue(m_sprite->getScaleY());
	pg->GetProperty(wxT("Width"))->SetValue(m_sprite->getSymbol().getWidth() * m_sprite->getScaleX());
	pg->GetProperty(wxT("Height"))->SetValue(m_sprite->getSymbol().getHeight() * m_sprite->getScaleY());
	pg->GetProperty(wxT("Shear X"))->SetValue(m_sprite->getShearX());
	pg->GetProperty(wxT("Shear Y"))->SetValue(m_sprite->getShearY());

	bool xMirror, yMirror;
	m_sprite->getMirror(xMirror, yMirror);
	pg->GetProperty(wxT("Horizontal Mirror"))->SetValue(xMirror);
	pg->GetProperty(wxT("Vertical Mirror"))->SetValue(yMirror);
}

void SpritePropertySetting::initProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_sprite->name));

	wxColour mul_col = wxColour(m_sprite->multiCol.r*255, m_sprite->multiCol.g*255, m_sprite->multiCol.b*255, m_sprite->multiCol.a*255);
	wxColour add_col = wxColour(m_sprite->addCol.r*255, m_sprite->addCol.g*255, m_sprite->addCol.b*255, m_sprite->addCol.a*255);
	pg->Append(new wxColourProperty(wxT("Multi Color"), wxPG_LABEL, mul_col));
	pg->SetPropertyAttribute("Multi Color", "HasAlpha", true);
	pg->Append(new wxColourProperty(wxT("Add Color"), wxPG_LABEL, add_col));
	pg->SetPropertyAttribute("Add Color", "HasAlpha", true);

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getPosition().x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getPosition().y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_sprite->getAngle() * TRANS_RAD_TO_DEG));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Scale X"), wxPG_LABEL, m_sprite->getScaleX()));
	pg->SetPropertyAttribute(wxT("Scale X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale X"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("Scale Y"), wxPG_LABEL, m_sprite->getScaleY()));
	pg->SetPropertyAttribute(wxT("Scale Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale Y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("Width"), wxPG_LABEL, m_sprite->getSymbol().getWidth() * m_sprite->getScaleX()));
	pg->SetPropertyAttribute(wxT("Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Width"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("Height"), wxPG_LABEL, m_sprite->getSymbol().getHeight() * m_sprite->getScaleY()));
	pg->SetPropertyAttribute(wxT("Height"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Height"), "Precision", 2);

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

void SpritePropertySetting::translate(float x, float y) 
{
	Vector new_pos(x, y);

	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new arrange_sprite::MoveSpritesAOP(sprites, new_pos - m_sprite->getPosition()));

	m_sprite->setTransform(new_pos, m_sprite->getAngle());
}

void SpritePropertySetting::rotate(float angle)
{
	float offset_angle = angle - m_sprite->getAngle();

	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new arrange_sprite::RotateSpritesAOP(sprites, offset_angle));

	m_sprite->setTransform(m_sprite->getPosition(), angle);	
}

void SpritePropertySetting::scale(float sx, float sy)
{
	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new arrange_sprite::ScaleSpritesAOP(sprites, sx, sy));

	m_sprite->setScale(sx, sy);
}

void SpritePropertySetting::shear(float kx, float ky)
{
	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new arrange_sprite::ShearSpritesAOP(sprites, kx, ky));

	m_sprite->setShear(kx, ky);
}

void SpritePropertySetting::mirror(bool mx, bool my)
{
	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new arrange_sprite::MirrorSpritesAOP(sprites, mx, my));

	m_sprite->setMirror(mx, my);
}

} // d2d