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
	else if (name == "Tag")
		m_sprite->tag = wxANY_AS(value, wxString);
	else if (name == wxT("Multi Color")) {
		wxColour col = wxANY_AS(value, wxColour);
		m_sprite->multiCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == wxT("Add Color")) {
		wxColour col = wxANY_AS(value, wxColour);
		m_sprite->addCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == wxT("Clip"))
		m_sprite->clip = wxANY_AS(value, bool);
	// pos
	else if (name == wxT("Pos"))
	{
		double x, y;
		splitString(value, &x, &y);
		translate(x, y);
	}
	else if (name == wxT("Pos.X"))
		translate(wxANY_AS(value, float), m_sprite->getPosition().y);
	else if (name == wxT("Pos.Y"))
		translate(m_sprite->getPosition().x, wxANY_AS(value, float));
	// angle
	else if (name == wxT("Angle"))
		rotate(wxANY_AS(value, float) * TRANS_DEG_TO_RAD);
	// scale
	else if (name == wxT("Scale"))
	{
		double x, y;
		splitString(value, &x, &y);
		scale(x, y);
	}
	else if (name == wxT("Scale.X"))
		scale(wxANY_AS(value, float), m_sprite->getScale().y);
	else if (name == wxT("Scale.Y"))
		scale(m_sprite->getScale().x, wxANY_AS(value, float));
	// size
	else if (name == wxT("Size"))
	{
		double w, h;
		splitString(value, &w, &h);
		scale(w/m_sprite->getSymbol().getSize().xLength(), h/m_sprite->getSymbol().getSize().yLength());
	}
	else if (name == wxT("Size.Width"))
	{
		const float width = wxANY_AS(value, float);
		scale(width/m_sprite->getSymbol().getSize().xLength(), m_sprite->getScale().y);
	}
	else if (name == wxT("Size.Height"))
	{
		const float height = wxANY_AS(value, float);
		scale(m_sprite->getScale().x, height/m_sprite->getSymbol().getSize().yLength());
	}
	// shear
	else if (name == wxT("Shear"))
	{
		double x, y;
		splitString(value, &x, &y);
		shear(x, y);
	}
	else if (name == wxT("Shear.X"))
		shear(wxANY_AS(value, float), m_sprite->getShear().y);
	else if (name == wxT("Shear.Y"))
		shear(m_sprite->getShear().x, wxANY_AS(value, float));
	// offset
	else if (name == wxT("Offset"))
	{
		double x, y;
		splitString(value, &x, &y);
		offset(x, y);
	}
	else if (name == wxT("Offset.X"))
		offset(wxANY_AS(value, float), m_sprite->getOffset().y);
	else if (name == wxT("Offset.Y"))
		offset(m_sprite->getOffset().x, wxANY_AS(value, float));
	// mirror
	else if (name == wxT("Mirror"))
	{
		// todo
	}
	else if (name == wxT("Mirror.Horizontal"))
	{
		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
//		m_sprite->setMirror(wxANY_AS(value, bool), yMirror);
		mirror(wxANY_AS(value, bool), yMirror);
	}
	else if (name == wxT("Mirror.Vertical"))
	{
		bool xMirror, yMirror;
		m_sprite->getMirror(xMirror, yMirror);
//		m_sprite->setMirror(xMirror, wxANY_AS(value, bool));
		mirror(xMirror, wxANY_AS(value, bool));
	}
	else if (name == wxT("Visiable"))
		m_sprite->visiable = wxANY_AS(value, bool);
	else if (name == wxT("Editable"))
		m_sprite->editable = wxANY_AS(value, bool);

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
	pg->GetProperty(wxT("Tag"))->Enable(bEnable);
	pg->GetProperty(wxT("Multi Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Add Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Clip"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos.X"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos.Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Angle"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale.X"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale.Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Size"))->Enable(bEnable);
	pg->GetProperty(wxT("Size.Width"))->Enable(bEnable);
	pg->GetProperty(wxT("Size.Height"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear.X"))->Enable(bEnable);
	pg->GetProperty(wxT("Shear.Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Offset"))->Enable(bEnable);
	pg->GetProperty(wxT("Offset.X"))->Enable(bEnable);
	pg->GetProperty(wxT("Offset.Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Mirror"))->Enable(bEnable);
	pg->GetProperty(wxT("Mirror.Horizontal"))->Enable(bEnable);
	pg->GetProperty(wxT("Mirror.Vertical"))->Enable(bEnable);

	pg->GetProperty(wxT("Visiable"))->Enable(bEnable);
	pg->GetProperty(wxT("Editable"))->Enable(bEnable);
}

void SpritePropertySetting::updateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_sprite->name);
	pg->GetProperty(wxT("Tag"))->SetValue(m_sprite->tag);

// 	pg->GetProperty(wxT("Multi Color"))->SetValue(m_sprite->multiColor);
// 	pg->GetProperty(wxT("Add Color"))->SetValue(m_sprite->addColor);

	wxColour mul_col = wxColour(m_sprite->multiCol.r*255, m_sprite->multiCol.g*255, m_sprite->multiCol.b*255, m_sprite->multiCol.a*255);
	wxColour add_col = wxColour(m_sprite->addCol.r*255, m_sprite->addCol.g*255, m_sprite->addCol.b*255, m_sprite->addCol.a*255);
	pg->SetPropertyValueString(wxT("Multi Color"), mul_col.GetAsString());
	pg->SetPropertyValueString(wxT("Add Color"), add_col.GetAsString());
	pg->GetProperty(wxT("Clip"))->SetValue(m_sprite->clip);

	pg->GetProperty(wxT("Pos.X"))->SetValue(m_sprite->getPosition().x);
	pg->GetProperty(wxT("Pos.Y"))->SetValue(m_sprite->getPosition().y);
	pg->GetProperty(wxT("Angle"))->SetValue(m_sprite->getAngle() * TRANS_RAD_TO_DEG);
	pg->GetProperty(wxT("Scale.X"))->SetValue(m_sprite->getScale().x);
	pg->GetProperty(wxT("Scale.Y"))->SetValue(m_sprite->getScale().y);
	pg->GetProperty(wxT("Size.Width"))->SetValue(m_sprite->getSymbol().getSize().xLength() * m_sprite->getScale().x);
	pg->GetProperty(wxT("Size.Height"))->SetValue(m_sprite->getSymbol().getSize().yLength() * m_sprite->getScale().y);
	pg->GetProperty(wxT("Shear.X"))->SetValue(m_sprite->getShear().x);
	pg->GetProperty(wxT("Shear.Y"))->SetValue(m_sprite->getShear().y);
	pg->GetProperty(wxT("Offset.X"))->SetValue(m_sprite->getShear().x);
	pg->GetProperty(wxT("Offset.Y"))->SetValue(m_sprite->getShear().y);

	bool xMirror, yMirror;
	m_sprite->getMirror(xMirror, yMirror);
	pg->GetProperty(wxT("Mirror.Horizontal"))->SetValue(xMirror);
	pg->GetProperty(wxT("Mirror.Vertical"))->SetValue(yMirror);

	pg->GetProperty(wxT("Visiable"))->SetValue(m_sprite->visiable);
	pg->GetProperty(wxT("Editable"))->SetValue(m_sprite->editable);
}

void SpritePropertySetting::initProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxPropertyCategory("INFO", wxPG_LABEL));

	pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_sprite->name));

	pg->Append(new wxStringProperty("Tag", wxPG_LABEL, m_sprite->tag));

	pg->Append(new wxBoolProperty("Clip", wxPG_LABEL, m_sprite->clip));

	wxColour mul_col = wxColour(m_sprite->multiCol.r*255, m_sprite->multiCol.g*255, m_sprite->multiCol.b*255, m_sprite->multiCol.a*255);
	wxColour add_col = wxColour(m_sprite->addCol.r*255, m_sprite->addCol.g*255, m_sprite->addCol.b*255, m_sprite->addCol.a*255);
	pg->Append(new wxColourProperty(wxT("Multi Color"), wxPG_LABEL, mul_col));
	pg->SetPropertyAttribute("Multi Color", "HasAlpha", true);
	pg->Append(new wxColourProperty(wxT("Add Color"), wxPG_LABEL, add_col));
	pg->SetPropertyAttribute("Add Color", "HasAlpha", true);

	pg->Append(new wxPropertyCategory("GEOMETRY", wxPG_LABEL));

	wxPGProperty* posProp = pg->Append(new wxStringProperty(wxT("Pos"), wxPG_LABEL, wxT("<composed>")));
	posProp->SetExpanded(false);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getPosition().x));
	pg->SetPropertyAttribute(wxT("Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.X"), "Precision", 1);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getPosition().y));
	pg->SetPropertyAttribute(wxT("Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_sprite->getAngle() * TRANS_RAD_TO_DEG));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	wxPGProperty* scaleProp = pg->Append(new wxStringProperty(wxT("Scale"), wxPG_LABEL, wxT("<composed>")));
	scaleProp->SetExpanded(false);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getScale().x));
	pg->SetPropertyAttribute(wxT("Scale.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.X"), "Precision", 2);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getScale().y));
	pg->SetPropertyAttribute(wxT("Scale.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.Y"), "Precision", 2);

	wxPGProperty* sizeProp = pg->Append(new wxStringProperty(wxT("Size"), wxPG_LABEL, wxT("<composed>")));
	sizeProp->SetExpanded(false);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Width"), wxPG_LABEL, m_sprite->getSymbol().getSize().xLength() * m_sprite->getScale().x));
	pg->SetPropertyAttribute(wxT("Size.Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Width"), "Precision", 2);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Height"), wxPG_LABEL, m_sprite->getSymbol().getSize().yLength() * m_sprite->getScale().y));
	pg->SetPropertyAttribute(wxT("Size.Height"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Height"), "Precision", 2);

	wxPGProperty* shearProp = pg->Append(new wxStringProperty(wxT("Shear"), wxPG_LABEL, wxT("<composed>")));
	shearProp->SetExpanded(false);
	pg->AppendIn(shearProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getShear().x));
	pg->SetPropertyAttribute(wxT("Shear.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Shear.X"), "Precision", 2);
	pg->AppendIn(shearProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getShear().y));
	pg->SetPropertyAttribute(wxT("Shear.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Shear.Y"), "Precision", 2);

	wxPGProperty* offsetProp = pg->Append(new wxStringProperty(wxT("Offset"), wxPG_LABEL, wxT("<composed>")));
	offsetProp->SetExpanded(false);
	pg->AppendIn(offsetProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, m_sprite->getOffset().x));
	pg->SetPropertyAttribute(wxT("Offset.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Offset.X"), "Precision", 1);
	pg->AppendIn(offsetProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_sprite->getOffset().y));
	pg->SetPropertyAttribute(wxT("Offset.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Offset.Y"), "Precision", 1);

	wxPGProperty* mirrorProp = pg->Append(new wxStringProperty(wxT("Mirror"), wxPG_LABEL, wxT("<composed>")));
	mirrorProp->SetExpanded(false);
	bool xMirror, yMirror;
	m_sprite->getMirror(xMirror, yMirror);
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Horizontal"), wxPG_LABEL, xMirror));
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Vertical"), wxPG_LABEL, yMirror));

	pg->Append(new wxPropertyCategory("EDIT", wxPG_LABEL));
	pg->Append(new wxBoolProperty("Visiable", wxPG_LABEL, m_sprite->visiable));
	pg->Append(new wxBoolProperty("Editable", wxPG_LABEL, m_sprite->editable));
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

void SpritePropertySetting::offset(float ox, float oy)
{
	m_sprite->setOffset(Vector(ox, oy));
}

void SpritePropertySetting::mirror(bool mx, bool my)
{
	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new arrange_sprite::MirrorSpritesAOP(sprites, mx, my));

	m_sprite->setMirror(mx, my);
}

void SpritePropertySetting::splitString(const wxAny& value, double* x, double* y)
{
	wxString val = wxANY_AS(value, wxString);
	size_t gap = val.find_first_of(';');
	wxString sx = val.substr(0, gap),
		sy = val.substr(gap + 1);
	sx.ToDouble(x);
	sy.ToDouble(y);
}

} // d2d