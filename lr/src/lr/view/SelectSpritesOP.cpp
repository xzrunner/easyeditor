#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "dataset/GroupHelper.h"
#include "view/typedef.h"

namespace lr
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(stage_wnd, stage, spritesImpl, callback)
	, m_open_symbol(stage_wnd, stage, spritesImpl)
{
	stage->SetCursor(wxCursor(wxCURSOR_PENCIL));

	m_first_press.setInvalid();
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (d2d::SelectSpritesOP::OnKeyDown(keyCode)) {
		return true;
	}

	if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'g' || keyCode == 'G'))
	{
		GroupSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'b' || keyCode == 'B'))
	{
		BreakUpSelection();
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->PointQuery(pos);

	if (m_first_press.isValid()) {
		stage->Pathfinding(m_first_press, pos);
		m_first_press.setInvalid();
	} else {
		m_first_press = pos;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (selected) {
		m_open_symbol.Open(selected);
	}

	return false;
}

void SelectSpritesOP::GroupSelection()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<d2d::ISprite*> sprites;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	d2d::ISprite* group = GroupHelper::Group(sprites);

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = sprites[i];
		d2d::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Release();
	}

	d2d::InsertSpriteSJ::Instance()->Insert(group);
	group->Release();
}

void SelectSpritesOP::BreakUpSelection()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<d2d::ISprite*> sprites;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		d2d::ISprite* spr = sprites[i];
		if (spr->GetSymbol().GetFilepath() != GROUP_TAG) {
			continue;
		}

		std::vector<d2d::ISprite*> children;
		GroupHelper::BreakUp(spr, children);
 		for (int j = 0, m = children.size(); j < m; ++j) {
			d2d::ISprite* spr = children[j];
 			d2d::InsertSpriteSJ::Instance()->Insert(spr);
			spr->Release();
 		}

		d2d::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Release();
	}
}

}