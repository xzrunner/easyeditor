#include "ArrangeSpriteOP.h"
#include "StagePanel.h"
#include "SpriteExt.h"

#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/SpriteSelection.h>

namespace sg
{

ArrangeSpriteOP::ArrangeSpriteOP(ee::PropertySettingPanel* property, StagePanel* stage)
	: ee::ArrangeSpriteFixOP(stage, stage->GetStageImpl(), stage, property, NULL, ee::ArrangeSpriteConfig(false, false, false, false))
{
}

bool ArrangeSpriteOP::OnMouseLeftUp(int x, int y)
{
	static_cast<StagePanel*>(m_wnd)->GetCheckBoard().ClearRemovedCache();
	return ee::ArrangeSpriteFixOP::OnMouseLeftUp(x, y);
}

bool ArrangeSpriteOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::ArrangeSpriteFixOP::OnMouseLeftDClick(x, y)) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = stage->QuerySpriteByPos(pos);
	if (!selected) {
		return false;
	}

	std::vector<ee::Sprite*> sprites;
	stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* s = sprites[i];
		if (&s->GetSymbol() == &selected->GetSymbol()) {
			stage->GetSpriteSelection()->Add(s);
		}
	}

	return false;
}

void ArrangeSpriteOP::onDirectionKeyDown(int type)
{
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* s = sprites[i];
		int row, col;
		stage->TransCoordsToGridPos(s->GetPosition(), row, col);
		switch (type)
		{
		case ee::KEY_UP:
			++row;
			break;
		case ee::KEY_DOWN:
			--row;
			break;
		case ee::KEY_LEFT:
			--col;
			break;
		case ee::KEY_RIGHT:
			++col;
			break;
		}

		sm::vec2 pos;
		stage->TransGridPosToCoords(row, col, pos);
		s->SetTransform(pos, s->GetAngle());
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ArrangeSpriteOP::PasteSprToClipboard(const ee::Sprite* spr, Json::Value& value) const
{
	ee::ArrangeSpriteFixOP::PasteSprToClipboard(spr, value);
	value["level"] = static_cast<SpriteExt*>(spr->GetUserData())->level;
}

void ArrangeSpriteOP::CopySprFromClipboard(ee::Sprite* spr, const Json::Value& value) const
{
	ee::ArrangeSpriteFixOP::CopySprFromClipboard(spr, value);
	SpriteExt* info = new SpriteExt;
	info->level = value["level"].asInt();
	spr->SetUserData(info);
}

}