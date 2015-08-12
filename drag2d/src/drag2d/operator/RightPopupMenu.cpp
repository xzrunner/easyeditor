#include "RightPopupMenu.h"

#include "common/config.h"
#include "common/visitors.h"
#include "common/sprite_visitors.h"
#include "common/FileNameTools.h"
#include "view/SpriteSelection.h"
#include "view/EditPanelImpl.h"
#include "view/IStageCanvas.h"
#include "view/MultiSpritesImpl.h"
#include "dataset/ISymbol.h"
#include "render/DynamicTexAndFont.h"

namespace d2d
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

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	CreateCommonMenu(menu);
	CreateSelectMenu(menu, x, y);
	CreateDebugMenu(menu);

	m_stage->PopupMenu(&menu, x, y);
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	if (id >= MENU_UP_MOST && id <= MENU_VERT_MIRROR) {
		HandleCommonMenu(id);
	} else if (id == MENU_INSERT_TO_DTEX || id == MENU_REMOVE_FROM_DTEX) {
		HandleDebugTagMenu(id);
	} else if (id >= MENU_MULTI_SELECTED && id <= MENU_MULTI_SELECTED_END) {
		HandleSelectMenu(id);
	}
}

void RightPopupMenu::CreateCommonMenu(wxMenu& menu)
{
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_UP_MOST);
	menu.Append(MENU_UP_MOST, "ÒÆµ½¶¥");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_DOWN_MOST);
	menu.Append(MENU_DOWN_MOST, "ÒÆµ½µ×");

	menu.AppendSeparator();

	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_HORI_MIRROR);
	menu.Append(MENU_HORI_MIRROR, "Ë®Æ½¾µÏñ");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_VERT_MIRROR);
	menu.Append(MENU_VERT_MIRROR, "ÊúÖ±¾µÏñ");	

	menu.AppendSeparator();
}

void RightPopupMenu::CreateSelectMenu(wxMenu& menu, int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);

	PointMultiQueryVisitor visitor0(pos);
	m_selection->Traverse(visitor0);
	const std::vector<ISprite*>& sprites0 = visitor0.GetResult();
	if (sprites0.size() == 1) {
		m_selection->Clear();
		m_selection->Add(sprites0[0]);
		return;
	}

	PointMultiQueryVisitor visitor1(pos);
	m_sprites_impl->TraverseSprites(visitor1);

	const std::vector<ISprite*>& sprites1 = visitor1.GetResult();
	if (sprites1.empty()) {
		return;
	}
	m_selection->Clear();
	if (sprites1.size() == 1) {
		m_selection->Add(sprites1[0]);
		return;
	}

	int sz = std::min(MENU_MULTI_SELECTED_END - MENU_MULTI_SELECTED + 1, (int)sprites1.size());
	for (int i = 0; i < sz; ++i) {
		ISprite* spr = sprites1[i];
		m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_MULTI_SELECTED + i);
		std::string name = FilenameTools::getFilename(spr->GetSymbol().GetFilepath());
		menu.Append(MENU_MULTI_SELECTED + i, name);
		m_selection->Add(spr);
	}

	menu.AppendSeparator();
}

void RightPopupMenu::CreateDebugMenu(wxMenu& menu)
{
#ifdef _DEBUG
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_INSERT_TO_DTEX);
	menu.Append(MENU_INSERT_TO_DTEX, "Insert To DTex");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanelImpl::OnRightPopupMenu, m_stage, MENU_REMOVE_FROM_DTEX);
	menu.Append(MENU_REMOVE_FROM_DTEX, "Remove From DTex");
#endif
}

void RightPopupMenu::HandleCommonMenu(int id)
{
	if (id == MENU_UP_MOST) {
		UpLayerMost();
	} else if (id == MENU_DOWN_MOST) {
		DownLayerMost();
	} else if (id == MENU_HORI_MIRROR) {
		HoriMirror();
	} else if (id == MENU_VERT_MIRROR) {
		VertMirror();
	}
}

void RightPopupMenu::HandleSelectMenu(int id)
{
	std::vector<ISprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));

	int idx = id - MENU_MULTI_SELECTED;
	if (idx < 0 || idx >= sprites.size()) {
		return;
	}

	ISprite* selected = sprites[idx];
	m_selection->Clear();
	m_selection->Add(selected);
}

void RightPopupMenu::HandleDebugTagMenu(int id)
{
	if (id == MENU_INSERT_TO_DTEX) {
		if (Config::Instance()->IsUseDTex()) {
			std::vector<ISprite*> selected;
			m_selection->Traverse(FetchAllVisitor<ISprite>(selected));

			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
			for (size_t i = 0, n = selected.size(); i < n; ++i) {
				ISymbol& s = const_cast<ISymbol&>(selected[i]->GetSymbol());
				dtex->InsertSymbol(s);
			}

			m_stage->GetCanvas()->ResetViewport();
		}
	} else if (id == MENU_REMOVE_FROM_DTEX) {
		if (Config::Instance()->IsUseDTex()) {
			std::vector<ISprite*> selected;
			m_selection->Traverse(FetchAllVisitor<ISprite>(selected));
			//DynamicTexture* dtex = DynamicTexture::Instance();
			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
			for (size_t i = 0, n = selected.size(); i < n; ++i) {
				ISymbol& s = const_cast<ISymbol&>(selected[i]->GetSymbol());
				dtex->Remove(s.GetFilepath());
			}
		}
	}
}

void RightPopupMenu::UpLayerMost()
{
	std::vector<ISprite*> selected;
	m_selection->Traverse(FetchAllVisitor<ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		do {
		} while (m_sprites_impl->ReorderSprite(selected[i], true));
	}
}

void RightPopupMenu::DownLayerMost()
{
	std::vector<ISprite*> selected;
	m_selection->Traverse(FetchAllVisitor<ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		do {
		} while (m_sprites_impl->ReorderSprite(selected[i], false));
	}
}

void RightPopupMenu::HoriMirror()
{
	std::vector<ISprite*> selected;
	m_selection->Traverse(FetchAllVisitor<ISprite>(selected));
	bool dirty = false;
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		ISprite* spr = selected[i];
		spr->SetMirror(!spr->GetMirrorX(), spr->GetMirrorY());
		dirty = true;
	}
	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

void RightPopupMenu::VertMirror()
{
	std::vector<ISprite*> selected;
	m_selection->Traverse(FetchAllVisitor<ISprite>(selected));
	bool dirty = false;
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		ISprite* spr = selected[i];
		spr->SetMirror(spr->GetMirrorX(), !spr->GetMirrorY());
		dirty = true;
	}
	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

}