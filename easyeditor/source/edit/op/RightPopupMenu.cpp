#include "RightPopupMenu.h"
#include "FileHelper.h"
#include "SpriteSelection.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "MultiSpritesImpl.h"
#include "Symbol.h"
#include "sprite_msg.h"
#include "panel_msg.h"
#include "FetchAllVisitor.h"
#include "PointMultiQueryVisitor.h"

namespace ee
{

RightPopupMenu::RightPopupMenu(wxWindow* parent,
							   EditPanelImpl* stage,
							   MultiSpritesImpl* sprite_impl,
							   SpriteSelection* selection)
	: m_parent(parent)
	, m_stage(stage)
	, m_sprites_impl(sprite_impl)
	, m_selection(selection)
{
}

RightPopupMenu::~RightPopupMenu()
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	OnSelected(x, y);

	CreateCommonMenu(menu);
	CreateSelectMenu(menu);
	CreateDebugMenu(menu);
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	if (id >= MENU_UP_LAYER && id <= MENU_COMMON_END) {
		HandleCommonMenu(id);
	} else if (id == MENU_INSERT_TO_DTEX || id == MENU_REMOVE_FROM_DTEX) {
		HandleDebugTagMenu(id);
	} else if (id >= MENU_MULTI_SELECTED && id <= MENU_MULTI_SELECTED_END) {
		HandleSelectMenu(id);
	}
}

void RightPopupMenu::CreateCommonMenu(wxMenu& menu)
{
	if (m_edited_sprs.empty()) {
		return;
	}

	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_UP_LAYER);
	menu.Append(MENU_UP_LAYER, "上移一层");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_DOWN_LAYER);
	menu.Append(MENU_DOWN_LAYER, "下移一层");	

	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_UP_MOST);
	menu.Append(MENU_UP_MOST, "移到顶");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_DOWN_MOST);
	menu.Append(MENU_DOWN_MOST, "移到底");

	menu.AppendSeparator();

	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_HORI_MIRROR);
	menu.Append(MENU_HORI_MIRROR, "水平镜像");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_VERT_MIRROR);
	menu.Append(MENU_VERT_MIRROR, "竖直镜像");	

	menu.AppendSeparator();

	if (m_edited_sprs.size() == 1) {
		m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_SELECT_SAME);
		menu.Append(MENU_SELECT_SAME, "选择相同");
		menu.AppendSeparator();
	}
}

void RightPopupMenu::CreateSelectMenu(wxMenu& menu)
{
	if (m_selected_sprs.size() <= 1) {
		return;
	}

	int sz = std::min(MENU_MULTI_SELECTED_END - MENU_MULTI_SELECTED + 1, (int)m_selected_sprs.size());
	for (int i = 0; i < sz; ++i) {
		Sprite* spr = m_selected_sprs[i];
		m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_MULTI_SELECTED + i);
		std::string name = FileHelper::GetFilename(spr->GetSymbol().GetFilepath());
		menu.Append(MENU_MULTI_SELECTED + i, name);
	}

	menu.AppendSeparator();
}

void RightPopupMenu::CreateDebugMenu(wxMenu& menu)
{
	if (m_edited_sprs.empty()) {
		return;
	}

#ifdef _DEBUG
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_INSERT_TO_DTEX);
	menu.Append(MENU_INSERT_TO_DTEX, "Insert To DTex");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_REMOVE_FROM_DTEX);
	menu.Append(MENU_REMOVE_FROM_DTEX, "Remove From DTex");

	menu.AppendSeparator();
#endif
}

void RightPopupMenu::HandleCommonMenu(int id)
{
	switch (id)
	{
	case MENU_UP_LAYER:
		UpOneLayer();
		break;
	case MENU_DOWN_LAYER:
		DownOneLayer();
		break;
	case MENU_UP_MOST:
		UpLayerMost();
		break;
	case MENU_DOWN_MOST:
		DownLayerMost();
		break;
	case MENU_HORI_MIRROR:
		HoriMirror();
		break;
	case MENU_VERT_MIRROR:
		VertMirror();
		break;
	case MENU_SELECT_SAME:
		SelectSame();
		break;
	}
}

void RightPopupMenu::HandleSelectMenu(int id)
{
	int idx = id - MENU_MULTI_SELECTED;
	if (idx < 0 || idx >= static_cast<int>(m_selected_sprs.size())) {
		return;
	}

	Sprite* selected = m_selected_sprs[idx];
	m_selection->Clear();
	m_selection->Add(selected);

	SelectSpriteSJ::Instance()->Select(selected, true);
}

void RightPopupMenu::HandleDebugTagMenu(int id)
{
// 	if (id == MENU_INSERT_TO_DTEX) {
// 		if (Config::Instance()->IsUseDTex()) {
// 			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
// 			for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
// 				Symbol& s = const_cast<Symbol&>(m_edited_sprs[i]->GetSymbol());
// 				dtex->InsertSymbol(s);
// 			}
// 		}
// 	} else if (id == MENU_REMOVE_FROM_DTEX) {
// 		if (Config::Instance()->IsUseDTex()) {
// 			//DynamicTexture* dtex = DynamicTexture::Instance();
// 			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
// 			for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
// 				Symbol& s = const_cast<Symbol&>(m_edited_sprs[i]->GetSymbol());
// 				dtex->Remove(s.GetFilepath());
// 			}
// 		}
// 	}
}

void RightPopupMenu::UpOneLayer()
{
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		ReorderSpriteSJ::Instance()->Reorder(m_edited_sprs[i], true);
	}
}

void RightPopupMenu::DownOneLayer()
{
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		ReorderSpriteSJ::Instance()->Reorder(m_edited_sprs[i], false);
	}
}

void RightPopupMenu::UpLayerMost()
{
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		ReorderSpriteMostSJ::Instance()->Reorder(m_edited_sprs[i], true);
	}
}

void RightPopupMenu::DownLayerMost()
{
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		ReorderSpriteMostSJ::Instance()->Reorder(m_edited_sprs[i], false);
	}
}

void RightPopupMenu::HoriMirror()
{
	bool dirty = false;
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		Sprite* spr = m_edited_sprs[i];
		spr->SetMirror(!spr->GetMirrorX(), spr->GetMirrorY());
		dirty = true;
	}
	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void RightPopupMenu::VertMirror()
{
	bool dirty = false;
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		Sprite* spr = m_edited_sprs[i];
		spr->SetMirror(spr->GetMirrorX(), !spr->GetMirrorY());
		dirty = true;
	}
	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void RightPopupMenu::SelectSame()
{
	std::vector<Sprite*> selected;
	m_selection->Traverse(FetchAllVisitor<Sprite>(selected));
	assert(selected.size() == 1);
 	std::string filepath = selected[0]->GetSymbol().GetFilepath();

	m_selection->Clear();
	std::vector<Sprite*> sprites;
	m_sprites_impl->TraverseSprites(FetchAllVisitor<Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Sprite* spr = sprites[i];
		if (spr->GetSymbol().GetFilepath() == filepath) {
			m_selection->Add(spr);
		}
	}
}

void RightPopupMenu::OnSelected(int x, int y)
{
	m_selected_sprs.clear();
	m_edited_sprs.clear();

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	PointMultiQueryVisitor visitor_selected(pos);
	m_selection->Traverse(visitor_selected);
	const std::vector<Sprite*>& sprites_selected = visitor_selected.GetResult();
	if (!sprites_selected.empty()) {
		m_selection->Traverse(FetchAllVisitor<Sprite>(m_edited_sprs));
	}

	PointMultiQueryVisitor visitor_all(pos);
	m_sprites_impl->TraverseSprites(visitor_all, DT_EDITABLE);
	m_selected_sprs = visitor_all.GetResult();

	m_selection->Clear();
	if (m_selected_sprs.size() == 1 && m_edited_sprs.empty()) {
		m_edited_sprs = m_selected_sprs;
		m_selected_sprs.clear();
		m_selection->Add(m_edited_sprs[0]);
	} else if (!m_edited_sprs.empty()) {
		m_selected_sprs.clear();
		for (int i = 0, n = m_edited_sprs.size(); i < n; ++i) {
			m_selection->Add(m_edited_sprs[i]);
		}
	} else {
		for (int i = 0, n = m_selected_sprs.size(); i < n; ++i) {
			m_selection->Add(m_selected_sprs[i]);
		}
	}
}

}