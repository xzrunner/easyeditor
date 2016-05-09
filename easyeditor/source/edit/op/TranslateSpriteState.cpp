#include "TranslateSpriteState.h"
#include "TranslateSpriteAOP.h"
#include "SpriteSelection.h"
#include "panel_msg.h"

namespace ee
{

TranslateSpriteState::TranslateSpriteState(SpriteSelection* selection, 
										   const sm::vec2& first_pos)
	: m_dirty(false)
{
	m_selection = selection;
	m_selection->Retain();

	m_first_pos = m_last_pos = first_pos;
	m_last_valid = true;
}

TranslateSpriteState::~TranslateSpriteState()
{
	m_selection->Release();
}

void TranslateSpriteState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
	m_last_valid = true;
}

void TranslateSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	m_last_valid = false;
	if (m_dirty) {
		m_dirty = false;
		AtomicOP* aop = new TranslateSpriteAOP(*m_selection, pos - m_first_pos);
		EditAddRecordSJ::Instance()->Add(aop);
	}
}

bool TranslateSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selection->IsEmpty() || !m_last_valid) {
		return false;
	}

	m_dirty = true;
	Translate(pos - m_last_pos);
	m_last_pos = pos;
	m_last_valid = true;

	return true;
}

bool TranslateSpriteState::OnDirectionKeyDown(DirectionType type)
{
	if (m_selection->IsEmpty()) return false;

	sm::vec2 offset;

	switch (type)
	{
	case e_left:
		offset.Set(-1, 0);
		break;
	case e_right:
		offset.Set(1, 0);
		break;
	case e_down:
		offset.Set(0, -1);
		break;
	case e_up:
		offset.Set(0, 1);
		break;
	}

	if (offset.x != 0 || offset.y != 0) {
		Translate(offset);
		EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(*m_selection, offset));
	}

	return true;
}

void TranslateSpriteState::Translate(const sm::vec2& offset)
{
	m_selection->Traverse(TranslateVisitor(offset));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::TranslateVisitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::TranslateVisitor::
Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);
	sprite->Translate(m_offset);
	next = true;
}

}