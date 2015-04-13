#include "SpritePropertySetting.h"
#include "PropertySettingPanel.h"

#include "history/TranslateSpriteAOP.h"
#include "history/RotateSpriteAOP.h"
#include "history/ScaleSpriteAOP.h"
#include "history/ShearSpriteAOP.h"
#include "history/MirrorSpriteAOP.h"
#include "history/OffsetSpriteAOP.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "common/Math.h"
#include "common/color_trans.h"
#include "widgets/ColorProperty.h"

#include <wx/propgrid/advprops.h>

namespace d2d
{

SpritePropertySetting::SpritePropertySetting(EditPanel* editPanel, ISprite* sprite)
	: IPropertySetting(editPanel, wxT("Sprite"))
{
	m_sprite = sprite;
	if (m_sprite) {
		m_sprite->Retain();
	}
}

SpritePropertySetting::~SpritePropertySetting()
{
	if (m_sprite) {
		m_sprite->Release();
	}	
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

	// info
	if (name == wxT("Name"))
	{
		m_sprite->name = wxANY_AS(value, wxString);
	}
	else if (name == "Tag")
	{
		m_sprite->tag = wxANY_AS(value, wxString);
	}
	else if (name == "Color.Multi")
	{
		wxColour col = wxANY_AS(value, wxColour);
		m_sprite->multiCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);

	}
	else if (name == "Color.Add")
	{
		wxColour col = wxANY_AS(value, wxColour);
		m_sprite->addCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == "Color.Alpha")
	{
		int alpha = wxANY_AS(value, int);
		alpha = std::max(0, std::min(255, alpha));
		m_sprite->multiCol.a = alpha / 255.0f;
	}
// 	else if (name == "Color.R")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		m_sprite->r_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
// 	else if (name == "Color.G")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		m_sprite->g_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
// 	else if (name == "Color.B")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		m_sprite->b_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
	else if (name == wxT("Clip"))
	{
		m_sprite->clip = wxANY_AS(value, bool);
	}
	// pos
	else if (name == wxT("Pos"))
	{
 		double x, y;
 		splitString(value, &x, &y);
 		translate(x, y);
	}
	// angle
	else if (name == wxT("Angle"))
	{
		rotate(wxANY_AS(value, float) * TRANS_DEG_TO_RAD);
	}
	// scale
	else if (name == wxT("Scale"))
	{
		double x, y;
		splitString(value, &x, &y);
		scale(x, y);
	}
	// size
	else if (name == wxT("Size"))
	{
		double w, h;
		splitString(value, &w, &h);
		scale(w/m_sprite->getSymbol().getSize().xLength(), h/m_sprite->getSymbol().getSize().yLength());
	}
	// shear
	else if (name == wxT("Shear"))
	{
		double x, y;
		splitString(value, &x, &y);
		shear(x, y);
	}
	// offset
	else if (name == wxT("Offset"))
	{
		double x, y;
		splitString(value, &x, &y);
		offset(x, y);
	}
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
	{
		m_sprite->visiable = wxANY_AS(value, bool);
	}
	else if (name == wxT("Editable"))
	{
		m_sprite->editable = wxANY_AS(value, bool);
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
	pg->GetProperty(wxT("Tag"))->Enable(bEnable);
	pg->GetProperty(wxT("Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Color.Multi"))->Enable(bEnable);
	pg->GetProperty(wxT("Color.Add"))->Enable(bEnable);
	pg->GetProperty(wxT("Color.Alpha"))->Enable(bEnable);
// 	pg->GetProperty(wxT("Color.R"))->Enable(bEnable);
// 	pg->GetProperty(wxT("Color.G"))->Enable(bEnable);
// 	pg->GetProperty(wxT("Color.B"))->Enable(bEnable);
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

	wxColour mul_col = wxColour(m_sprite->multiCol.r*255, m_sprite->multiCol.g*255, m_sprite->multiCol.b*255, m_sprite->multiCol.a*255);
	wxColour add_col = wxColour(m_sprite->addCol.r*255, m_sprite->addCol.g*255, m_sprite->addCol.b*255, m_sprite->addCol.a*255);
	pg->SetPropertyValueString(wxT("Color.Multi"), mul_col.GetAsString());
	pg->SetPropertyValueString(wxT("Color.Add"), add_col.GetAsString());
	pg->GetProperty(wxT("Color.Alpha"))->SetValue((int)(m_sprite->multiCol.a*255));

// 	wxColour r_trans = wxColour(m_sprite->r_trans.r*255, m_sprite->r_trans.g*255, m_sprite->r_trans.b*255, m_sprite->r_trans.a*255);
// 	wxColour g_trans = wxColour(m_sprite->g_trans.r*255, m_sprite->g_trans.g*255, m_sprite->g_trans.b*255, m_sprite->g_trans.a*255);
// 	wxColour b_trans = wxColour(m_sprite->b_trans.r*255, m_sprite->b_trans.g*255, m_sprite->b_trans.b*255, m_sprite->b_trans.a*255);
// 	pg->SetPropertyValueString(wxT("Color.R"), r_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.G"), g_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.B"), b_trans.GetAsString());

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
 	pg->GetProperty(wxT("Offset.X"))->SetValue(m_sprite->getOffset().x);
	pg->GetProperty(wxT("Offset.Y"))->SetValue(m_sprite->getOffset().y);

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
	wxPGProperty* colProp = pg->Append(new wxStringProperty(wxT("Color"), wxPG_LABEL, wxT("<composed>")));
	colProp->SetExpanded(false);
	pg->AppendIn(colProp, new wxColourProperty(wxT("Multi"), wxPG_LABEL, mul_col));
	pg->SetPropertyAttribute(wxT("Color.Multi"), "HasAlpha", true);
	pg->AppendIn(colProp, new wxColourProperty(wxT("Add"), wxPG_LABEL, add_col));
	pg->SetPropertyAttribute(wxT("Color.Add"), "HasAlpha", true);
	pg->AppendIn(colProp, new wxIntProperty(wxT("Alpha"), wxPG_LABEL, m_sprite->multiCol.a*255));
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Max", 255);

	wxPGProperty* col_conv_prop = pg->Append(new wxStringProperty(wxT("Color Conversion"), wxPG_LABEL, wxT("<composed>")));
	col_conv_prop->SetExpanded(false);

	ColorProperty* col_r_prop = new ColorProperty("R");
	col_r_prop->SetColor(&m_sprite->r_trans);
	col_r_prop->SetStage(m_editPanel);
	pg->AppendIn(col_conv_prop, col_r_prop);

	ColorProperty* col_g_prop = new ColorProperty("G");
	col_g_prop->SetColor(&m_sprite->g_trans);
	col_g_prop->SetStage(m_editPanel);
	pg->AppendIn(col_conv_prop, col_g_prop);

	ColorProperty* col_b_prop = new ColorProperty("B");
	col_b_prop->SetColor(&m_sprite->b_trans);
	col_b_prop->SetStage(m_editPanel);
	pg->AppendIn(col_conv_prop, col_b_prop);

// 	wxColour r_trans = wxColour(m_sprite->r_trans.r*255, m_sprite->r_trans.g*255, m_sprite->r_trans.b*255, m_sprite->r_trans.a*255);
// 	wxColour g_trans = wxColour(m_sprite->g_trans.r*255, m_sprite->g_trans.g*255, m_sprite->g_trans.b*255, m_sprite->g_trans.a*255);
// 	wxColour b_trans = wxColour(m_sprite->b_trans.r*255, m_sprite->b_trans.g*255, m_sprite->b_trans.b*255, m_sprite->b_trans.a*255);
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("R"), wxPG_LABEL, r_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("G"), wxPG_LABEL, g_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("B"), wxPG_LABEL, b_trans));

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
	m_editPanel->addHistoryOP(new TranslateSpriteAOP(sprites, new_pos - m_sprite->getPosition()));

	m_sprite->setTransform(new_pos, m_sprite->getAngle());
}

void SpritePropertySetting::rotate(float angle)
{
	float offset_angle = angle - m_sprite->getAngle();

	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editPanel->addHistoryOP(new RotateSpriteAOP(sprites, offset_angle));

	m_sprite->setTransform(m_sprite->getPosition(), angle);	
}

void SpritePropertySetting::scale(float sx, float sy)
{
	m_editPanel->addHistoryOP(new ScaleSpriteAOP(m_sprite, 
		Vector(sx, sy), m_sprite->getScale()));
	m_sprite->setScale(sx, sy);
}

void SpritePropertySetting::shear(float kx, float ky)
{
	m_editPanel->addHistoryOP(new ShearSpriteAOP(m_sprite, 
		Vector(kx, ky), m_sprite->getShear()));
	m_sprite->setShear(kx, ky);
}

void SpritePropertySetting::offset(float ox, float oy)
{
	m_editPanel->addHistoryOP(new OffsetSpriteAOP(m_sprite, Vector(ox, oy), m_sprite->getOffset()));
	m_sprite->setOffset(Vector(ox, oy));
}

void SpritePropertySetting::mirror(bool mx, bool my)
{
	m_editPanel->addHistoryOP(new MirrorSpriteAOP(m_sprite, 
		m_sprite->getMirrorX(), m_sprite->getMirrorY(), mx, my));
	m_sprite->setMirror(mx, my);
}

} // d2d