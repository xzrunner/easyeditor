#include "TranslateSpriteState.h"

#include "history/TranslateSpriteAOP.h"
#include "view/SpriteSelection.h"

namespace d2d
{

TranslateSpriteState::TranslateSpriteState(SpriteSelection* selection, 
										   const Vector& first_pos)
	: m_dirty(false)
{
	m_selection = selection;
	m_selection->Retain();

	m_first_pos = m_last_pos = first_pos;
}

TranslateSpriteState::~TranslateSpriteState()
{
	m_selection->Release();
}

void TranslateSpriteState::OnMousePress(const Vector& pos)
{
	m_first_pos = m_last_pos = pos;
}

AbstractAtomicOP* TranslateSpriteState::OnMouseRelease(const Vector& pos)
{
	m_last_pos.setInvalid();
	if (m_dirty) {
		m_dirty = false;
		return new TranslateSpriteAOP(*m_selection, pos - m_first_pos);
	} else {
		return NULL;
	}
}

bool TranslateSpriteState::OnMouseDrag(const Vector& pos)
{
	if (m_selection->IsEmpty() || !m_last_pos.isValid()) {
		return false;
	}

	m_dirty = true;
	Translate(pos - m_last_pos);
	m_last_pos = pos;

	return true;
}

bool TranslateSpriteState::OnDirectionKeyDown(DirectionType type)
{
	if (m_selection->IsEmpty()) return false;

	switch (type)
	{
	case e_left:
		Translate(Vector(-1, 0));
		break;
	case e_right:
		Translate(Vector(1, 0));
		break;
	case e_down:
		Translate(Vector(0, -1));
		break;
	case e_up:
		Translate(Vector(0, 1));
		break;
	}

	return true;
}

void TranslateSpriteState::Translate(const Vector& offset)
{
	m_selection->Traverse(Visitor(offset));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	sprite->translate(m_offset);
	bFetchNext = true;
}

}