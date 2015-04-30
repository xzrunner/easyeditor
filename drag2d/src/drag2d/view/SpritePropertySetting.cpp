#include "SpritePropertySetting.h"
#include "SpritePropertyImpl.h"
#include "PropertySettingPanel.h"
#include "PropertyColorMonitor.h"

#include "common/Math.h"
#include "common/color_trans.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "widgets/ColorProperty.h"

#include <wx/propgrid/advprops.h>

namespace d2d
{

SpritePropertySetting::SpritePropertySetting(EditPanel* editPanel, ISprite* sprite)
	: IPropertySetting(editPanel, wxT("Sprite"))
	, m_impl(new SpritePropertyImpl(editPanel, sprite))
{
}

SpritePropertySetting::~SpritePropertySetting()
{
	delete m_impl;
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

	d2d::ISprite* spr = m_impl->GetSprite();

	// info
	if (name == wxT("Name"))
	{
		spr->name = wxANY_AS(value, wxString);
	}
	else if (name == "Tag")
	{
		spr->tag = wxANY_AS(value, wxString);
	}
	else if (name == "Color.Multi")
	{
		wxColour col = wxANY_AS(value, wxColour);
		spr->multiCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);

	}
	else if (name == "Color.Add")
	{
		wxColour col = wxANY_AS(value, wxColour);
		spr->addCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == "Color.Alpha")
	{
		int alpha = wxANY_AS(value, int);
		alpha = std::max(0, std::min(255, alpha));
		spr->multiCol.a = alpha / 255.0f;
	}
// 	else if (name == "Color.R")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->r_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
// 	else if (name == "Color.G")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->g_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
// 	else if (name == "Color.B")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->b_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
	else if (name == wxT("Clip"))
	{
		spr->clip = wxANY_AS(value, bool);
	}
	// pos
	else if (name == wxT("Pos"))
	{
 		double x, y;
 		splitString(value, &x, &y);
 		m_impl->Translate(x, y);
	}
	// angle
	else if (name == wxT("Angle"))
	{
		m_impl->Rotate(wxANY_AS(value, float) * TRANS_DEG_TO_RAD);
	}
	// scale
	else if (name == wxT("Scale"))
	{
		double x, y;
		splitString(value, &x, &y);
		m_impl->Scale(x, y);
	}
	// size
	else if (name == wxT("Size"))
	{
		double w, h;
		splitString(value, &w, &h);
		m_impl->Scale(w/spr->getSymbol().getSize().xLength(), h/spr->getSymbol().getSize().yLength());
	}
	// shear
	else if (name == wxT("Shear"))
	{
		double x, y;
		splitString(value, &x, &y);
		m_impl->Shear(x, y);
	}
	// offset
	else if (name == wxT("Offset"))
	{
		double x, y;
		splitString(value, &x, &y);
		m_impl->Offset(x, y);
	}
	// mirror
	else if (name == wxT("Mirror"))
	{
		// todo
	}
	else if (name == wxT("Mirror.Horizontal"))
	{
		bool xMirror, yMirror;
		spr->getMirror(xMirror, yMirror);
//		spr->setMirror(wxANY_AS(value, bool), yMirror);
		m_impl->Mirror(wxANY_AS(value, bool), yMirror);
	}
	else if (name == wxT("Mirror.Vertical"))
	{
		bool xMirror, yMirror;
		spr->getMirror(xMirror, yMirror);
//		spr->setMirror(xMirror, wxANY_AS(value, bool));
		m_impl->Mirror(xMirror, wxANY_AS(value, bool));
	}
	else if (name == wxT("Visiable"))
	{
		spr->visiable = wxANY_AS(value, bool);
	}
	else if (name == wxT("Editable"))
	{
		spr->editable = wxANY_AS(value, bool);
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
	d2d::ISprite* spr = m_impl->GetSprite();

	pg->GetProperty(wxT("Name"))->SetValue(spr->name);
	pg->GetProperty(wxT("Tag"))->SetValue(spr->tag);

	wxColour mul_col = wxColour(spr->multiCol.r*255, spr->multiCol.g*255, spr->multiCol.b*255, spr->multiCol.a*255);
	wxColour add_col = wxColour(spr->addCol.r*255, spr->addCol.g*255, spr->addCol.b*255, spr->addCol.a*255);
	pg->SetPropertyValueString(wxT("Color.Multi"), mul_col.GetAsString());
	pg->SetPropertyValueString(wxT("Color.Add"), add_col.GetAsString());
	pg->GetProperty(wxT("Color.Alpha"))->SetValue((int)(spr->multiCol.a*255));

// 	wxColour r_trans = wxColour(spr->r_trans.r*255, spr->r_trans.g*255, spr->r_trans.b*255, spr->r_trans.a*255);
// 	wxColour g_trans = wxColour(spr->g_trans.r*255, spr->g_trans.g*255, spr->g_trans.b*255, spr->g_trans.a*255);
// 	wxColour b_trans = wxColour(spr->b_trans.r*255, spr->b_trans.g*255, spr->b_trans.b*255, spr->b_trans.a*255);
// 	pg->SetPropertyValueString(wxT("Color.R"), r_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.G"), g_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.B"), b_trans.GetAsString());

	ColorProperty* rp = static_cast<ColorProperty*>(pg->GetProperty("Color Conversion.R"));
	rp->SetListener(new PropertyColorListener(m_editPanel, &spr->r_trans));

	ColorProperty* gp = static_cast<ColorProperty*>(pg->GetProperty("Color Conversion.G"));
	gp->SetListener(new PropertyColorListener(m_editPanel, &spr->g_trans));

	ColorProperty* bp = static_cast<ColorProperty*>(pg->GetProperty("Color Conversion.B"));
	bp->SetListener(new PropertyColorListener(m_editPanel, &spr->b_trans));

	pg->GetProperty(wxT("Clip"))->SetValue(spr->clip);

	pg->GetProperty(wxT("Pos.X"))->SetValue(spr->getPosition().x);
	pg->GetProperty(wxT("Pos.Y"))->SetValue(spr->getPosition().y);
	pg->GetProperty(wxT("Angle"))->SetValue(spr->getAngle() * TRANS_RAD_TO_DEG);
	pg->GetProperty(wxT("Scale.X"))->SetValue(spr->getScale().x);
	pg->GetProperty(wxT("Scale.Y"))->SetValue(spr->getScale().y);
	pg->GetProperty(wxT("Size.Width"))->SetValue(spr->getSymbol().getSize().xLength() * spr->getScale().x);
	pg->GetProperty(wxT("Size.Height"))->SetValue(spr->getSymbol().getSize().yLength() * spr->getScale().y);
	pg->GetProperty(wxT("Shear.X"))->SetValue(spr->getShear().x);
	pg->GetProperty(wxT("Shear.Y"))->SetValue(spr->getShear().y);
 	pg->GetProperty(wxT("Offset.X"))->SetValue(spr->getOffset().x);
	pg->GetProperty(wxT("Offset.Y"))->SetValue(spr->getOffset().y);

	bool xMirror, yMirror;
	spr->getMirror(xMirror, yMirror);
	pg->GetProperty(wxT("Mirror.Horizontal"))->SetValue(xMirror);
	pg->GetProperty(wxT("Mirror.Vertical"))->SetValue(yMirror);

	pg->GetProperty(wxT("Visiable"))->SetValue(spr->visiable);
	pg->GetProperty(wxT("Editable"))->SetValue(spr->editable);
}

void SpritePropertySetting::initProperties(wxPropertyGrid* pg)
{
	d2d::ISprite* spr = m_impl->GetSprite();

	pg->Clear();

	pg->Append(new wxPropertyCategory("INFO", wxPG_LABEL));

	pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, spr->name));

	pg->Append(new wxStringProperty("Tag", wxPG_LABEL, spr->tag));

	pg->Append(new wxBoolProperty("Clip", wxPG_LABEL, spr->clip));

	wxPGProperty* colProp = pg->Append(new wxStringProperty(wxT("Color"), wxPG_LABEL, wxT("<composed>")));
	wxColour mul_col = wxColour(spr->multiCol.r*255, spr->multiCol.g*255, spr->multiCol.b*255, spr->multiCol.a*255);
	wxColour add_col = wxColour(spr->addCol.r*255, spr->addCol.g*255, spr->addCol.b*255, spr->addCol.a*255);
	colProp->SetExpanded(false);
	pg->AppendIn(colProp, new wxColourProperty(wxT("Multi"), wxPG_LABEL, mul_col));
	pg->SetPropertyAttribute(wxT("Color.Multi"), "HasAlpha", true);
	pg->AppendIn(colProp, new wxColourProperty(wxT("Add"), wxPG_LABEL, add_col));
	pg->SetPropertyAttribute(wxT("Color.Add"), "HasAlpha", true);
	pg->AppendIn(colProp, new wxIntProperty(wxT("Alpha"), wxPG_LABEL, spr->multiCol.a*255));
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Max", 255);

	wxPGProperty* col_conv_prop = pg->Append(new wxStringProperty(wxT("Color Conversion"), wxPG_LABEL, wxT("<composed>")));
	col_conv_prop->SetExpanded(false);

	ColorProperty* col_r_prop = new ColorProperty("R");
	col_r_prop->SetListener(new PropertyColorListener(m_editPanel, &spr->r_trans));
	pg->AppendIn(col_conv_prop, col_r_prop);

	ColorProperty* col_g_prop = new ColorProperty("G");
	col_g_prop->SetListener(new PropertyColorListener(m_editPanel, &spr->g_trans));
	pg->AppendIn(col_conv_prop, col_g_prop);

	ColorProperty* col_b_prop = new ColorProperty("B");
	col_b_prop->SetListener(new PropertyColorListener(m_editPanel, &spr->b_trans));
	pg->AppendIn(col_conv_prop, col_b_prop);

// 	wxColour r_trans = wxColour(spr->r_trans.r*255, spr->r_trans.g*255, spr->r_trans.b*255, spr->r_trans.a*255);
// 	wxColour g_trans = wxColour(spr->g_trans.r*255, spr->g_trans.g*255, spr->g_trans.b*255, spr->g_trans.a*255);
// 	wxColour b_trans = wxColour(spr->b_trans.r*255, spr->b_trans.g*255, spr->b_trans.b*255, spr->b_trans.a*255);
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("R"), wxPG_LABEL, r_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("G"), wxPG_LABEL, g_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("B"), wxPG_LABEL, b_trans));

	pg->Append(new wxPropertyCategory("GEOMETRY", wxPG_LABEL));

	wxPGProperty* posProp = pg->Append(new wxStringProperty(wxT("Pos"), wxPG_LABEL, wxT("<composed>")));
	posProp->SetExpanded(false);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->getPosition().x));
	pg->SetPropertyAttribute(wxT("Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.X"), "Precision", 1);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->getPosition().y));
	pg->SetPropertyAttribute(wxT("Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, spr->getAngle() * TRANS_RAD_TO_DEG));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	wxPGProperty* scaleProp = pg->Append(new wxStringProperty(wxT("Scale"), wxPG_LABEL, wxT("<composed>")));
	scaleProp->SetExpanded(false);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->getScale().x));
	pg->SetPropertyAttribute(wxT("Scale.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.X"), "Precision", 2);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->getScale().y));
	pg->SetPropertyAttribute(wxT("Scale.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.Y"), "Precision", 2);

	wxPGProperty* sizeProp = pg->Append(new wxStringProperty(wxT("Size"), wxPG_LABEL, wxT("<composed>")));
	sizeProp->SetExpanded(false);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Width"), wxPG_LABEL, spr->getSymbol().getSize().xLength() * spr->getScale().x));
	pg->SetPropertyAttribute(wxT("Size.Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Width"), "Precision", 2);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Height"), wxPG_LABEL, spr->getSymbol().getSize().yLength() * spr->getScale().y));
	pg->SetPropertyAttribute(wxT("Size.Height"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Height"), "Precision", 2);

	wxPGProperty* shearProp = pg->Append(new wxStringProperty(wxT("Shear"), wxPG_LABEL, wxT("<composed>")));
	shearProp->SetExpanded(false);
	pg->AppendIn(shearProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->getShear().x));
	pg->SetPropertyAttribute(wxT("Shear.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Shear.X"), "Precision", 2);
	pg->AppendIn(shearProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->getShear().y));
	pg->SetPropertyAttribute(wxT("Shear.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Shear.Y"), "Precision", 2);

	wxPGProperty* offsetProp = pg->Append(new wxStringProperty(wxT("Offset"), wxPG_LABEL, wxT("<composed>")));
	offsetProp->SetExpanded(false);
	pg->AppendIn(offsetProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->getOffset().x));
	pg->SetPropertyAttribute(wxT("Offset.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Offset.X"), "Precision", 1);
	pg->AppendIn(offsetProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->getOffset().y));
	pg->SetPropertyAttribute(wxT("Offset.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Offset.Y"), "Precision", 1);

	wxPGProperty* mirrorProp = pg->Append(new wxStringProperty(wxT("Mirror"), wxPG_LABEL, wxT("<composed>")));
	mirrorProp->SetExpanded(false);
	bool xMirror, yMirror;
	spr->getMirror(xMirror, yMirror);
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Horizontal"), wxPG_LABEL, xMirror));
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Vertical"), wxPG_LABEL, yMirror));

	pg->Append(new wxPropertyCategory("EDIT", wxPG_LABEL));
	pg->Append(new wxBoolProperty("Visiable", wxPG_LABEL, spr->visiable));
	pg->Append(new wxBoolProperty("Editable", wxPG_LABEL, spr->editable));
}

ISprite* SpritePropertySetting::GetSprite()
{
	if (m_impl) {
		return m_impl->GetSprite();
	} else {
		return NULL;
	}
}

} // d2d