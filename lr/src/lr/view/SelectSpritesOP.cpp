#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "dataset/NewComplexUD.h"
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
#include <ee/StringHelper.h>
#include <ee/FileType.h>

#include <easycomplex.h>

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

	// group
	if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'G') {
		BuildGroup();
		return true;
	} else if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'B') {
		BreakUpGroup();
		return true;
	}
	// complex
	else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'G') {
		BuildComplex();
		return true;
	} else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'B') {
		BreakUpComplex();
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

void SelectSpritesOP::BuildGroup()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

	ecomplex::Sprite* spr = ecomplex::GroupHelper::Group(sprites);
	ecomplex::Symbol& sym = const_cast<ecomplex::Symbol&>(spr->GetSymbol());
	sym.SetFilepath(GROUP_TAG);
	sym.name = "_group";
	sym.SetName("_group");
	spr->SetName("_group");

	ee::AtomicOP* move_op = new ee::TranslateSpriteAOP(sprites, -spr->GetPosition());

	std::vector<ee::Sprite*> removed;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->AddReference();
		removed.push_back(spr);
		spr->RemoveReference();
	}
	ee::AtomicOP* del_op = new ee::DeleteSpriteAOP(removed);

	ee::InsertSpriteSJ::Instance()->Insert(spr);
	ee::AtomicOP* add_op = new ee::InsertSpriteAOP(spr);
	spr->RemoveReference();

	ee::CombineAOP* combine = new ee::CombineAOP;
	combine->Insert(move_op);
	combine->Insert(del_op);
	combine->Insert(add_op);
	ee::EditAddRecordSJ::Instance()->Add(combine);
}

void SelectSpritesOP::BreakUpGroup()
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

		ee::SelectSpriteSJ::Instance()->Select(spr, true);

		std::vector<ee::Sprite*> children;
		ecomplex::GroupHelper::BreakUp(spr, children);
 		for (int j = 0, m = children.size(); j < m; ++j) {
			ee::Sprite* spr = children[j];
 			ee::InsertSpriteSJ::Instance()->Insert(spr);
			spr->RemoveReference();
 		}

		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->RemoveReference();
	}
}

void SelectSpritesOP::BuildComplex()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

	ecomplex::Sprite* spr = ecomplex::GroupHelper::Group(sprites);
	spr->SetUserData(new NewComplexUD());

	ecomplex::Symbol& sym = const_cast<ecomplex::Symbol&>(spr->GetSymbol());
	std::string filepath = static_cast<StagePanel*>(m_wnd)->GetResDir();
	filepath += "\\_tmp_";
	filepath += ee::StringHelper::ToString(wxDateTime::Now().GetTicks());
	filepath += "_" + ee::FileType::GetTag(ee::FileType::e_complex) + ".json";
	sym.SetFilepath(filepath);

	ee::InsertSpriteSJ::Instance()->Insert(spr);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::RemoveSpriteSJ::Instance()->Remove(sprites[i]);
	}
}

void SelectSpritesOP::BreakUpComplex()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::string tag = "_" + ee::FileType::GetTag(ee::FileType::e_complex) + ".json";
	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprites[i];
		if (spr->GetSymbol().GetFilepath().find(tag) == std::string::npos) {
			continue;
		}

		ee::SelectSpriteSJ::Instance()->Select(spr, true);

		std::vector<ee::Sprite*> children;
		ecomplex::GroupHelper::BreakUp(spr, children);
		for (int j = 0, m = children.size(); j < m; ++j) {
			ee::Sprite* spr = children[j];
			ee::InsertSpriteSJ::Instance()->Insert(spr);
			spr->RemoveReference();
		}

		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->RemoveReference();
	}
}

}