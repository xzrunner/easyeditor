#include "ArrangeSpriteOP.h"
#include "StagePanel.h"
#include "SpriteExt.h"

namespace sg
{

ArrangeSpriteOP::ArrangeSpriteOP(d2d::PropertySettingPanel* property, 
								 d2d::ViewPanelMgr* view_panel_mgr,
								 StagePanel* stage)
	: d2d::ArrangeSpriteFixOP(stage, stage, property, view_panel_mgr, NULL, d2d::ArrangeSpriteConfig(false, false, false, false))
{
}

bool ArrangeSpriteOP::OnMouseLeftUp(int x, int y)
{
	static_cast<StagePanel*>(m_stage)->GetCheckBoard().ClearRemovedCache();
	return d2d::ArrangeSpriteFixOP::OnMouseLeftUp(x, y);
}

bool ArrangeSpriteOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::ArrangeSpriteFixOP::OnMouseLeftDClick(x, y)) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_stage);

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* selected = stage->QuerySpriteByPos(pos);
	if (!selected) {
		return false;
	}

	std::vector<d2d::ISprite*> sprites;
	stage->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* s = sprites[i];
		if (&s->getSymbol() == &selected->getSymbol()) {
			stage->GetSpriteSelection()->Add(s);
		}
	}

	return false;
}

void ArrangeSpriteOP::onDirectionKeyDown(d2d::DirectionType type)
{
	StagePanel* stage = static_cast<StagePanel*>(m_stage);

	std::vector<d2d::ISprite*> sprites;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* s = sprites[i];
		int row, col;
		stage->TransCoordsToGridPos(s->getPosition(), row, col);
		switch (type)
		{
		case d2d::e_up:
			++row;
			break;
		case d2d::e_down:
			--row;
			break;
		case d2d::e_left:
			--col;
			break;
		case d2d::e_right:
			++col;
			break;
		}

		d2d::Vector pos;
		stage->TransGridPosToCoords(row, col, pos);
		s->setTransform(pos, s->getAngle());
	}

	m_stage->GetCanvas()->SetDirty();
}

void ArrangeSpriteOP::PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const
{
	d2d::ArrangeSpriteFixOP::PasteSprToClipboard(spr, value);
	value["level"] = static_cast<SpriteExt*>(spr->GetUserData())->level;
}

void ArrangeSpriteOP::CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const
{
	d2d::ArrangeSpriteFixOP::CopySprFromClipboard(spr, value);
	SpriteExt* info = new SpriteExt;
	info->level = value["level"].asInt();
	spr->SetUserData(info);
}

}