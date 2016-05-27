#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "dataset/GroupHelper.h"
#include "view/typedef.h"

#include <ee/EditPanelImpl.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/sprite_msg.h>
#include <ee/Symbol.h>
#include <ee/panel_msg.h>
#include <ee/DeleteSpriteAOP.h>
#include <ee/InsertSpriteAOP.h>
#include <ee/CombineAOP.h>
#include <ee/SpriteFactory.h>
#include <ee/PointQueryVisitor.h>
#include <ee/TranslateSpriteAOP.h>

namespace lr
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(stage_wnd, stage, sprites_impl, callback)
	, m_open_symbol(stage_wnd, stage, sprites_impl)
{
	m_first_press.MakeInvalid();
	stage->SetCursor(wxCursor(wxCURSOR_PENCIL));
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) {
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
	if (ee::SelectSpritesOP::OnMouseLeftDown(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->PointQuery(pos);

	if (m_first_press.IsValid()) {
		stage->Pathfinding(m_first_press, pos);
		m_first_press.MakeInvalid();
	} else {
		m_first_press = pos;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	ee::Sprite* selected = NULL;
	m_selection->Traverse(ee::PointQueryVisitor(pos, &selected));
	if (!selected) {
		selected = m_spritesImpl->QuerySpriteByPos(pos);
	}
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

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

	ee::Sprite* group = GroupHelper::Group(sprites, "_group");
	ee::SpriteFactory::Instance()->Insert(group);

	ee::AtomicOP* move_op = new ee::TranslateSpriteAOP(sprites, -group->GetPosition());

	std::vector<ee::Sprite*> removed;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Retain();
		removed.push_back(spr);
		spr->Release();
	}
	ee::AtomicOP* del_op = new ee::DeleteSpriteAOP(removed);

	ee::InsertSpriteSJ::Instance()->Insert(group);
	ee::AtomicOP* add_op = new ee::InsertSpriteAOP(group);
	group->Release();

	ee::CombineAOP* combine = new ee::CombineAOP;
	combine->Insert(move_op);
	combine->Insert(del_op);
	combine->Insert(add_op);
	ee::EditAddRecordSJ::Instance()->Add(combine);
}

void SelectSpritesOP::BreakUpSelection()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprites[i];
		if (spr->GetSymbol().GetFilepath() != GROUP_TAG) {
			continue;
		}

		std::vector<ee::Sprite*> children;
		GroupHelper::BreakUp(spr, children);
 		for (int j = 0, m = children.size(); j < m; ++j) {
			ee::Sprite* spr = children[j];
 			ee::InsertSpriteSJ::Instance()->Insert(spr);
			spr->Release();
 		}

		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Release();
	}
}

}