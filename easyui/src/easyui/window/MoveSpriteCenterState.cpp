#include "MoveSpriteCenterState.h"

#include <ee/Sprite.h>

#include <easycomplex.h>

namespace eui
{
namespace window
{

MoveSpriteCenterState::MoveSpriteCenterState(ecomplex::Sprite* spr,
											 const ee::Vector& first_pos)
	: m_spr(spr)
	, m_dirty(false)
{
	m_first_pos = m_last_pos = first_pos;
}

MoveSpriteCenterState::~MoveSpriteCenterState()
{
}

void MoveSpriteCenterState::OnMousePress(const ee::Vector& pos)
{
	m_first_pos = m_last_pos = pos;
}

void MoveSpriteCenterState::OnMouseRelease(const ee::Vector& pos)
{
	m_last_pos.SetInvalid();
	// todo edit history

	if (m_dirty && m_spr) {
		ecomplex::Symbol& complex = const_cast<ecomplex::Symbol&>(m_spr->GetSymbol());
		complex.InitBounding();
		m_spr->BuildBounding();
	}

	m_dirty = false;
}

bool MoveSpriteCenterState::OnMouseDrag(const ee::Vector& pos)
{
	if (!m_spr || !m_last_pos.IsValid()) {
		return false;
	}

	ee::Vector offset = m_last_pos - pos;
	m_last_pos = pos;

	m_dirty = true;

	ecomplex::Symbol& sym = const_cast<ecomplex::Symbol&>(m_spr->GetSymbol());
	for (int i = 0, n = sym.m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sym.m_sprites[i];
		spr->Translate(offset);
	}

	ecomplex::FileStorer::Store(sym.GetFilepath().c_str(), &sym);

	return true;
}

}
}