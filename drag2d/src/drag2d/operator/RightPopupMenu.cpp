#include "RightPopupMenu.h"

#include "common/config.h"
#include "common/visitors.h"
#include "common/sprite_visitors.h"
#include "common/FileNameTools.h"
#include "view/SpriteSelection.h"
#include "view/EditPanelImpl.h"
#include "view/IStageCanvas.h"
#include "view/MultiSpritesImpl.h"
#include "view/ViewPanelMgr.h"
#include "dataset/ISymbol.h"
#include "render/DynamicTexAndFont.h"
#include "message/SpriteSelectedSJ.h"

namespace d2d
{

RightPopupMenu::RightPopupMenu(wxWindow* parent,
							   EditPanelImpl* stage,
							   MultiSpritesImpl* sprite_impl,
							   SpriteSelection* selection,
							   ViewPanelMgr* view_panel_mgr)
	: m_parent(parent)
	, m_stage(stage)
	, m_sprites_impl(sprite_impl)
	, m_selection(selection)
	, m_view_panel_mgr(view_panel_mgr)
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
	if (m_edited_sprs.empty()) {
		return;
	}

	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_UP_MOST);
	menu.Append(MENU_UP_MOST, "ÒÆµ½¶¥");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_DOWN_MOST);
	menu.Append(MENU_DOWN_MOST, "ÒÆµ½µ×");

	menu.AppendSeparator();

	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_HORI_MIRROR);
	menu.Append(MENU_HORI_MIRROR, "Ë®Æ½¾µÏñ");
	m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_VERT_MIRROR);
	menu.Append(MENU_VERT_MIRROR, "ÊúÖ±¾µÏñ");	

	menu.AppendSeparator();
}

void RightPopupMenu::CreateSelectMenu(wxMenu& menu)
{
	if (m_selected_sprs.size() <= 1) {
		return;
	}

	int sz = std::min(MENU_MULTI_SELECTED_END - MENU_MULTI_SELECTED + 1, (int)m_selected_sprs.size());
	for (int i = 0; i < sz; ++i) {
		ISprite* spr = m_selected_sprs[i];
		m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanelImpl::OnRightPopupMenu, m_stage, MENU_MULTI_SELECTED + i);
		std::string name = FilenameTools::getFilename(spr->GetSymbol().GetFilepath());
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
	int idx = id - MENU_MULTI_SELECTED;
	if (idx < 0 || idx >= m_selected_sprs.size()) {
		return;
	}

	ISprite* selected = m_selected_sprs[idx];
	m_selection->Clear();
	m_selection->Add(selected);

	if (m_view_panel_mgr) {
		SpriteSelectedSJ::Params p;
		p.spr = selected;
		p.clear = true;
		SpriteSelectedSJ::Instance()->OnSelected(p);
	}
}

void RightPopupMenu::HandleDebugTagMenu(int id)
{
	if (id == MENU_INSERT_TO_DTEX) {
		if (Config::Instance()->IsUseDTex()) {
			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
			for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
				ISymbol& s = const_cast<ISymbol&>(m_edited_sprs[i]->GetSymbol());
				dtex->InsertSymbol(s);
			}

			m_stage->GetCanvas()->ResetViewport();
		}
	} else if (id == MENU_REMOVE_FROM_DTEX) {
		if (Config::Instance()->IsUseDTex()) {
			//DynamicTexture* dtex = DynamicTexture::Instance();
			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
			for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
				ISymbol& s = const_cast<ISymbol&>(m_edited_sprs[i]->GetSymbol());
				dtex->Remove(s.GetFilepath());
			}
		}
	}
}

void RightPopupMenu::UpLayerMost()
{
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		do {
		} while (m_sprites_impl->ReorderSprite(m_edited_sprs[i], true));
	}
}

void RightPopupMenu::DownLayerMost()
{
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		do {
		} while (m_sprites_impl->ReorderSprite(m_edited_sprs[i], false));
	}
}

void RightPopupMenu::HoriMirror()
{
	bool dirty = false;
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		ISprite* spr = m_edited_sprs[i];
		spr->SetMirror(!spr->GetMirrorX(), spr->GetMirrorY());
		dirty = true;
	}
	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

void RightPopupMenu::VertMirror()
{
	bool dirty = false;
	for (size_t i = 0, n = m_edited_sprs.size(); i < n; ++i) {
		ISprite* spr = m_edited_sprs[i];
		spr->SetMirror(spr->GetMirrorX(), !spr->GetMirrorY());
		dirty = true;
	}
	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

void RightPopupMenu::OnSelected(int x, int y)
{
	m_selected_sprs.clear();
	m_edited_sprs.clear();

	Vector pos = m_stage->TransPosScrToProj(x, y);

	PointMultiQueryVisitor visitor_selected(pos);
	m_selection->Traverse(visitor_selected);
	const std::vector<ISprite*>& sprites_selected = visitor_selected.GetResult();
	if (!sprites_selected.empty()) {
		m_selection->Traverse(FetchAllVisitor<ISprite>(m_edited_sprs));
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