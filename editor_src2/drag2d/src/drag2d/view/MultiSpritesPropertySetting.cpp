#include "MultiSpritesPropertySetting.h"
#include "PropertySettingPanel.h"

#include <algorithm>

#include "dataset/AbstractBV.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "common/Math.h"

namespace d2d
{

MultiSpritesPropertySetting::MultiSpritesPropertySetting(EditPanel* editPanel, const std::vector<ISprite*>& sprites)
	: IPropertySetting(editPanel, wxT("MultiSprites"))
	, m_sprites(sprites)
{
}

void MultiSpritesPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
		updateProperties(pg);
	else
		initProperties(pg);
}

void MultiSpritesPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Align"))
	{
		align(value);
	}
	else if (name == wxT("Center"))
	{
		center(value);
	}
	else if (name == wxT("All Pos.X"))
	{
		float x = wxANY_AS(value, float);
		for (int i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* s = m_sprites[i];
			s->setTransform(Vector(x, s->getPosition().y), s->getAngle());
		}
	}
	else if (name == wxT("All Pos.Y"))
	{
		float y = wxANY_AS(value, float);
		for (int i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* s = m_sprites[i];
			s->setTransform(Vector(s->getPosition().x, y), s->getAngle());
		}
	}
	else if (name == wxT("Pos"))
	{
		double dx, dy;
		splitString(value, &dx, &dy);
		for (int i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* s = m_sprites[i];
			d2d::Vector pos = s->getPosition();
			pos.x += dx;
			pos.y += dy;
			s->setTransform(pos, s->getAngle());
		}
	}
	else if (name == wxT("Angle"))
	{
		float angle = wxANY_AS(value, float) * TRANS_DEG_TO_RAD;
		for (int i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* s = m_sprites[i];
			s->setTransform(s->getPosition(), s->getAngle() + angle);
		}
	}
	else if (name == wxT("Scale"))
	{
		double dx, dy;
		splitString(value, &dx, &dy);
		for (int i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* s = m_sprites[i];
			d2d::Vector scale = s->getScale();
			scale.x *= dx;
			scale.y *= dy;
			s->setScale(scale.x, scale.y);
		}
	}

	m_editPanel->Refresh();
}

void MultiSpritesPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void MultiSpritesPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
		initProperties(pg);

	pg->GetProperty(wxT("Align"))->Enable(bEnable);
	pg->GetProperty(wxT("Center"))->Enable(bEnable);
	pg->GetProperty(wxT("All Pos"))->Enable(bEnable);
	pg->GetProperty(wxT("All Pos.X"))->Enable(bEnable);
	pg->GetProperty(wxT("All Pos.Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos.dx"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos.dy"))->Enable(bEnable);
	pg->GetProperty(wxT("Angle"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale.dx"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale.dy"))->Enable(bEnable);
}

void MultiSpritesPropertySetting::updateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Align"))->SetValue(wxT("none"));
	pg->GetProperty(wxT("Center"))->SetValue(wxT("none"));
	Vector pos = getSamePosition();
	pg->GetProperty(wxT("All Pos.X"))->SetValue(pos.x);
	pg->GetProperty(wxT("All Pos.Y"))->SetValue(pos.y);

	pg->GetProperty(wxT("Pos.dx"))->SetValue(0);
	pg->GetProperty(wxT("Pos.dy"))->SetValue(0);
	pg->GetProperty(wxT("Angle"))->SetValue(0);
	pg->GetProperty(wxT("Scale.dx"))->SetValue(1);
	pg->GetProperty(wxT("Scale.dy"))->SetValue(1);
}

void MultiSpritesPropertySetting::initProperties(wxPropertyGrid* pg)
{
	Vector pos = getSamePosition();

	pg->Clear();

	pg->Append(new wxPropertyCategory("POSITION", wxPG_LABEL));

	static const wxChar* align_labels[] = { wxT("none"),
		wxT("left"), wxT("right"), wxT("up"), wxT("down"), wxT("center x"), wxT("center y"), NULL };
	pg->Append(new wxEnumProperty(wxT("Align"), wxPG_LABEL, align_labels));

	static const wxChar* center_labels[] = { wxT("none"), 
		wxT("horizontal"), wxT("vertical"), wxT("relative"), NULL };
	pg->Append(new wxEnumProperty(wxT("Center"), wxPG_LABEL, center_labels));

	wxPGProperty* allPosProp = pg->Append(new wxStringProperty(wxT("All Pos"), wxPG_LABEL, wxT("<composed>")));
	allPosProp->SetExpanded(false);
	pg->AppendIn(allPosProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, pos.x));
	pg->SetPropertyAttribute(wxT("All Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("All Pos.X"), "Precision", 1);
	pg->AppendIn(allPosProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, pos.y));
	pg->SetPropertyAttribute(wxT("All Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("All Pos.Y"), "Precision", 1);

	pg->Append(new wxPropertyCategory("CHANGE", wxPG_LABEL));

	wxPGProperty* posProp = pg->Append(new wxStringProperty(wxT("Pos"), wxPG_LABEL, wxT("<composed>")));
	posProp->SetExpanded(false);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("dx"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("Pos.dx"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.dx"), "Precision", 1);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("dy"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("Pos.dy"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.dy"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	wxPGProperty* scaleProp = pg->Append(new wxStringProperty(wxT("Scale"), wxPG_LABEL, wxT("<composed>")));
	scaleProp->SetExpanded(false);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("dx"), wxPG_LABEL, 1));
	pg->SetPropertyAttribute(wxT("Scale.dx"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.dx"), "Precision", 2);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("dy"), wxPG_LABEL, 1));
	pg->SetPropertyAttribute(wxT("Scale.dy"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.dy"), "Precision", 2);
}

void MultiSpritesPropertySetting::align(const wxAny& value)
{
	int type = wxANY_AS(value, int);
	if (type == 1)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float l = sprite->getPosition().x - sprite->getSymbol().getSize().xLength()*0.5f;
			if (l < left)
				left = l;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float x = left + sprite->getSymbol().getSize().xLength()*0.5f;
			m_sprites[i]->setTransform(Vector(x, sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (type == 2)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float r = sprite->getPosition().x + sprite->getSymbol().getSize().xLength()*0.5f;
			if (r > right)
				right = r;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float x = right - sprite->getSymbol().getSize().xLength()*0.5f;
			m_sprites[i]->setTransform(Vector(x, sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (type == 3)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float u = sprite->getPosition().y + sprite->getSymbol().getSize().yLength()*0.5f;
			if (u > up)
				up = u;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float y = up - sprite->getSymbol().getSize().yLength()*0.5f;
			m_sprites[i]->setTransform(Vector(sprite->getPosition().x, y), sprite->getAngle());
		}
	}
	else if (type == 4)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float d = sprite->getPosition().y - sprite->getSymbol().getSize().yLength()*0.5f;
			if (d < down)
				down = d;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float y = down + sprite->getSymbol().getSize().yLength()*0.5f;
			m_sprites[i]->setTransform(Vector(sprite->getPosition().x, y), sprite->getAngle());
		}
	}
	else if (type == 5)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float y = m_sprites[i]->getPosition().y;
			if (y < down)
				down = y;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			m_sprites[i]->setTransform(Vector(sprite->getPosition().x, down), sprite->getAngle());
		}
	}
	else if (type == 6)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float x = m_sprites[i]->getPosition().x;
			if (x < left)
				left = x;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			m_sprites[i]->setTransform(Vector(left, sprite->getPosition().y), sprite->getAngle());
		}
	}
}

void MultiSpritesPropertySetting::center(const wxAny& value)
{
	int type = wxANY_AS(value, int);
	if (type == 1)
	{
		float left = FLT_MAX, right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float x = sprite->getPosition().x;
			if (x < left) left = x;
			if (x > right) right = x;
		}

		std::vector<ISprite*> sorted(m_sprites);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_x));
		const float space = (right - left) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			ISprite* sprite = sorted[i];
			sprite->setTransform(Vector(left + space * i, sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (type == 2)
	{
		float down = FLT_MAX, up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float y = sprite->getPosition().y;
			if (y < down) down = y;
			if (y > up) up = y;
		}

		std::vector<ISprite*> sorted(m_sprites);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_y));
		const float space = (up - down) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			ISprite* sprite = sorted[i];
			sprite->setTransform(Vector(sprite->getPosition().x, down + space * i), sprite->getAngle());
		}
	}
	else if (type == 3)
	{
		ISprite* base = NULL;
		float maxArea = 0;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float area = m_sprites[i]->getBounding()->area();
			if (area > maxArea)
			{
				maxArea = area;
				base = m_sprites[i];
			}
		}

		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			if (sprite != base)
				sprite->setTransform(base->getPosition(), sprite->getAngle());
		}
	}
}

Vector MultiSpritesPropertySetting::getSamePosition() const
{
	if (m_sprites.empty()) {
		Vector p;
		p.setInvalid();
		return p;
	}

	Vector p = m_sprites[0]->getPosition();
	for (size_t i = 1, n = m_sprites.size(); i < n; ++i)
	{
		if (p != m_sprites[i]->getPosition())
		{
			p.setInvalid();
			break;
		}
	}
	return p;
}
} // d2d