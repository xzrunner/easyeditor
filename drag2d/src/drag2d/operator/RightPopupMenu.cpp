#include "RightPopupMenu.h"

#include "common/config.h"
#include "common/visitors.h"
#include "view/SpriteSelection.h"
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
	, m_sprite(NULL)
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, ISprite* spr)
{
	m_sprite = spr;
	CreateCommonMenu(menu);
	CreateSelectMenu(menu);
	CreateDebugMenu(menu);
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	if (id == MENU_UP_MOST) {
		UpLayerMost();
	} else if (id == MENU_DOWN_MOST) {
		DownLayerMost();
	} else if (id == MENU_HORI_MIRROR) {
		HoriMirror();
	} else if (id == MENU_VERT_MIRROR) {
		VertMirror();
	} else if (id == MENU_INSERT_TO_DTEX) {
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

void RightPopupMenu::CreateCommonMenu(wxMenu& menu)
{
	
}

void RightPopupMenu::CreateSelectMenu(wxMenu& menu)
{

}

void RightPopupMenu::CreateDebugMenu(wxMenu& menu)
{

}

void RightPopupMenu::HandleCommonMenu(int id)
{

}

void RightPopupMenu::HandleSelectMenu(int id)
{

}

void RightPopupMenu::HandleDebugTagMenu(int id)
{

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