#include "MultiSpritesPropertySetting.h"
#include "PropertySettingPanel.h"

#include <algorithm>

#include "dataset/AbstractBV.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"

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
	else if (name == wxT("X"))
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			sprite->setTransform(Vector(wxANY_AS(value, float), sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (name == wxT("Y"))
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			sprite->setTransform(Vector(sprite->getPosition().x, wxANY_AS(value, float)), sprite->getAngle());
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
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
}

void MultiSpritesPropertySetting::updateProperties(wxPropertyGrid* pg)
{
	Vector pos = getSamePosition();
	pg->GetProperty(wxT("Align"))->SetValue(wxT("none"));
	pg->GetProperty(wxT("Center"))->SetValue(wxT("none"));
	pg->GetProperty(wxT("X"))->SetValue(pos.x);
	pg->GetProperty(wxT("Y"))->SetValue(pos.y);
}

void MultiSpritesPropertySetting::initProperties(wxPropertyGrid* pg)
{
	Vector pos = getSamePosition();

	pg->Clear();

	static const wxChar* align_labels[] = { wxT("none"),
		wxT("left"), wxT("right"), wxT("up"), wxT("down"), wxT("center x"), wxT("center y"), NULL };
	pg->Append(new wxEnumProperty(wxT("Align"), wxPG_LABEL, align_labels));

	static const wxChar* center_labels[] = { wxT("none"), 
		wxT("horizontal"), wxT("vertical"), wxT("relative"), NULL };
	pg->Append(new wxEnumProperty(wxT("Center"), wxPG_LABEL, center_labels));

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, pos.x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, pos.y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);
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