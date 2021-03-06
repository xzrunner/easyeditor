#include "SelectFixtureOP.h"
#include "StagePanel.h"
#include "FixturePropertySetting.h"
#include "WorldPropertySetting.h"
#include "DrawUtils.h"

#include <ee/EditPanelImpl.h>
#include <ee/Sprite.h>
#include <ee/panel_msg.h>
#include <ee/PropertySettingPanel.h>

namespace emodeling
{

SelectFixtureOP::SelectFixtureOP(StagePanel* editPanel, ee::PropertySettingPanel* property)
	: ee::DrawRectangleOP(editPanel, editPanel->GetStageImpl(), true)
	, m_selected(NULL)
	, m_mouseOn(NULL)
	, m_stagePanel(editPanel)
	, m_property(property)
{
	m_first_pos.MakeInvalid();
}

SelectFixtureOP::~SelectFixtureOP()
{
	
}

bool SelectFixtureOP::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	auto& spr = m_stagePanel->QuerySpriteByPos(pos);

	ee::PropertySetting* setting = NULL;
	if (spr)
	{
		m_selected = NULL;
		Body* body = static_cast<Body*>(spr->GetUserData());
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

	if (m_first_pos.IsValid())
	{
		sm::rect rect(m_first_pos, m_stage->TransPosScrToProj(x, y));
		std::vector<ee::SprPtr> sprs;
		m_stagePanel->QuerySpritesByRect(rect, sprs);

		m_selected = NULL;
		for (size_t i = 0, n = sprs.size(); i < n; ++i)
		{
			Body* body = static_cast<Body*>(sprs[i]->GetUserData());
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

		m_first_pos.MakeInvalid();
	}

	return false;
}

bool SelectFixtureOP::OnMouseMove(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	auto& spr = static_cast<StagePanel*>(m_wnd)->QuerySpriteByPos(pos);
	if (spr)
	{
		Body* body = static_cast<Body*>(spr->GetUserData());
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

	m_first_pos.MakeInvalid();

	m_selected = m_mouseOn = NULL;

	return false;
}

}