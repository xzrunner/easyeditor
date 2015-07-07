
#include "SelectFixtureOP.h"
#include "StagePanel.h"
#include "FixturePropertySetting.h"
#include "WorldPropertySetting.h"
#include "DrawUtils.h"

using namespace emodeling;

SelectFixtureOP::SelectFixtureOP(StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel)
	: d2d::DrawRectangleOP(editPanel, true)
	, m_selected(NULL)
	, m_mouseOn(NULL)
	, m_stagePanel(editPanel)
	, m_propertyPanel(propertyPanel)
{
	m_firstPos.setInvalid();
}

SelectFixtureOP::~SelectFixtureOP()
{
	
}

bool SelectFixtureOP::OnMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* sprite = m_stagePanel->querySpriteByPos(pos);

	d2d::IPropertySetting* setting = NULL;
	if (sprite)
	{
		m_selected = NULL;
		libmodeling::Body* body = static_cast<libmodeling::Body*>(sprite->GetUserData());
		for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
		{
			if (body->fixtures[i]->isContain(pos))
			{
				m_selected = body->fixtures[i];
				break;
			}
		}
		if (m_selected)
		{
			setting = new FixturePropertySetting(m_stage, m_selected);
		}
		else
		{
			d2d::DrawRectangleOP::OnMouseLeftDown(x, y);
			m_firstPos = pos;
			m_stage->Refresh();
		}
	}

	if (!setting)
		setting = new WorldPropertySetting(m_stage);
	m_propertyPanel->SetPropertySetting(setting);

	return false;
}

bool SelectFixtureOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		d2d::Rect rect(m_firstPos, m_stage->TransPosScrToProj(x, y));
		std::vector<d2d::ISprite*> sprites;
		m_stagePanel->querySpritesByRect(rect, sprites);

		m_selected = NULL;
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			libmodeling::Body* body = static_cast<libmodeling::Body*>(sprites[i]->GetUserData());
			for (size_t j = 0, m = body->fixtures.size(); j < m; ++j)
			{
				if (body->fixtures[j]->isIntersect(rect))
				{
					m_selected = body->fixtures[i];
					break;
				}
			}
		}

		if (m_selected)
			m_propertyPanel->SetPropertySetting(new FixturePropertySetting(m_stage, m_selected));
		else
			m_propertyPanel->SetPropertySetting(new WorldPropertySetting(m_stage));

		m_firstPos.setInvalid();
	}

	return false;
}

bool SelectFixtureOP::OnMouseMove(int x, int y)
{
	if (d2d::DrawRectangleOP::OnMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* sprite = static_cast<StagePanel*>(m_stage)->querySpriteByPos(pos);
	if (sprite)
	{
		libmodeling::Body* body = static_cast<libmodeling::Body*>(sprite->GetUserData());
		if (body)
		{
			for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
			{
				if (body->fixtures[i]->isContain(pos))
				{
					m_mouseOn = body->fixtures[i];
					break;
				}
			}
		}
	}

	m_stage->Refresh();

	return false;
}

bool SelectFixtureOP::OnDraw() const
{
	if (d2d::DrawRectangleOP::OnDraw()) return true;

	if (m_selected)
		DrawUtils::drawFixture(m_selected, DrawUtils::e_selected, true);
	if (m_mouseOn)
		DrawUtils::drawFixture(m_mouseOn, DrawUtils::e_mouseOn, true);

	return false;
}

bool SelectFixtureOP::Clear()
{
	if (d2d::DrawRectangleOP::Clear()) return true;

	m_firstPos.setInvalid();

	m_selected = m_mouseOn = NULL;

	return false;
}
