#include "SelectFixtureOP.h"
#include "StagePanel.h"
#include "FixturePropertySetting.h"
#include "WorldPropertySetting.h"
#include "DrawUtils.h"

#include <ee/EditPanelImpl.h>
#include <ee/Sprite.h>
#include <ee/panel_msg.h>
#include <ee/PropertySettingPanel.h>
#include <ee/Rect.h>

namespace emodeling
{

SelectFixtureOP::SelectFixtureOP(StagePanel* editPanel, ee::PropertySettingPanel* property)
	: ee::DrawRectangleOP(editPanel, editPanel->GetStageImpl(), true)
	, m_selected(NULL)
	, m_mouseOn(NULL)
	, m_stagePanel(editPanel)
	, m_property(property)
	, m_first_pos_valid(false)
{
}

SelectFixtureOP::~SelectFixtureOP()
{
	
}

bool SelectFixtureOP::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* sprite = m_stagePanel->QuerySpriteByPos(pos);

	ee::PropertySetting* setting = NULL;
	if (sprite)
	{
		m_selected = NULL;
		Body* body = static_cast<Body*>(sprite->GetUserData());
		for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
		{
			if (body->m_fixtures[i]->IsContain(pos))
			{
				m_selected = body->m_fixtures[i];
				break;
			}
		}
		if (m_selected)
		{
			setting = new FixturePropertySetting(m_stage, m_selected);
		}
		else
		{
			ee::DrawRectangleOP::OnMouseLeftDown(x, y);
			m_first_pos = pos;
			m_first_pos_valid = true;
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	if (!setting)
		setting = new WorldPropertySetting(m_stage);
	m_property->SetPropertySetting(setting);

	return false;
}

bool SelectFixtureOP::OnMouseLeftUp(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	if (m_first_pos_valid)
	{
		ee::Rect rect(m_first_pos, m_stage->TransPosScrToProj(x, y));
		std::vector<ee::Sprite*> sprites;
		m_stagePanel->QuerySpritesByRect(rect, sprites);

		m_selected = NULL;
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			Body* body = static_cast<Body*>(sprites[i]->GetUserData());
			for (size_t j = 0, m = body->m_fixtures.size(); j < m; ++j)
			{
				if (body->m_fixtures[j]->IsIntersect(rect))
				{
					m_selected = body->m_fixtures[i];
					break;
				}
			}
		}

		if (m_selected)
			m_property->SetPropertySetting(new FixturePropertySetting(m_stage, m_selected));
		else
			m_property->SetPropertySetting(new WorldPropertySetting(m_stage));

		m_first_pos_valid = false;
	}

	return false;
}

bool SelectFixtureOP::OnMouseMove(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* sprite = static_cast<StagePanel*>(m_wnd)->QuerySpriteByPos(pos);
	if (sprite)
	{
		Body* body = static_cast<Body*>(sprite->GetUserData());
		if (body)
		{
			for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
			{
				if (body->m_fixtures[i]->IsContain(pos))
				{
					m_mouseOn = body->m_fixtures[i];
					break;
				}
			}
		}
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool SelectFixtureOP::OnDraw() const
{
	if (ee::DrawRectangleOP::OnDraw()) return true;

	if (m_selected)
		DrawUtils::DrawFixture(m_selected, DrawUtils::e_selected, true);
	if (m_mouseOn)
		DrawUtils::DrawFixture(m_mouseOn, DrawUtils::e_mouseOn, true);

	return false;
}

bool SelectFixtureOP::Clear()
{
	if (ee::DrawRectangleOP::Clear()) return true;

	m_first_pos_valid = false;

	m_selected = m_mouseOn = NULL;

	return false;
}

}