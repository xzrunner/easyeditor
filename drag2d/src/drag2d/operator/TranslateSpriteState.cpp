#include "TranslateSpriteState.h"

#include "history/TranslateSpriteAOP.h"
#include "view/SpriteSelection.h"
#include "message/panel_msg.h"

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

void TranslateSpriteState::OnMouseRelease(const Vector& pos)
{
	m_last_pos.setInvalid();
	if (m_dirty) {
		m_dirty = false;
		AbstractAtomicOP* aop = new TranslateSpriteAOP(*m_selection, pos - m_first_pos);
		EditAddRecordSJ::Instance()->Add(aop);
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

	d2d::Vector offset;

	switch (type)
	{
	case e_left:
		offset.set(-1, 0);
		break;
	case e_right:
		offset.set(1, 0);
		break;
	case e_down:
		offset.set(0, -1);
		break;
	case e_up:
		offset.set(0, 1);
		break;
	}

	if (offset.x != 0 || offset.y != 0) {
		Translate(offset);
		EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(*m_selection, offset));
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
	sprite->Translate(m_offset);
	bFetchNext = true;
}

}