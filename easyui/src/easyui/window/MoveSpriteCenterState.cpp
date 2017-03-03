#include "MoveSpriteCenterState.h"

#include <ee/Sprite.h>
#include <ee/FileHelper.h>

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

namespace eui
{
namespace window
{

MoveSpriteCenterState::MoveSpriteCenterState(ecomplex::Sprite* spr,
											 const sm::vec2& first_pos)
	: m_spr(spr)
	, m_dirty(false)
{
	m_first_pos = m_last_pos = first_pos;
}

MoveSpriteCenterState::~MoveSpriteCenterState()
{
}

void MoveSpriteCenterState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void MoveSpriteCenterState::OnMouseRelease(const sm::vec2& pos)
{
	m_last_pos.MakeInvalid();
	// todo edit history

	if (m_dirty && m_spr) {
		m_spr->UpdateBounding();
	}

	m_dirty = false;
}

bool MoveSpriteCenterState::OnMouseDrag(const sm::vec2& pos)
{
	if (!m_spr || !m_last_pos.IsValid()) {
		return false;
	}

	sm::vec2 offset = m_last_pos - pos;
	m_last_pos = pos;

	m_dirty = true;

	const ecomplex::Symbol* sym = dynamic_cast<const ecomplex::Symbol*>(m_spr->GetSymbol());
	const std::vector<s2::Sprite*>& children = sym->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->Translate(-offset);
	}

	ecomplex::FileStorer::Store(sym->GetFilepath(), sym, ee::FileHelper::GetFileDir(sym->GetFilepath()));

	return true;
}

}
}