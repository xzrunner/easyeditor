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
}

TranslateSpriteState::~TranslateSpriteState()
{
	m_selection->Release();
}

void TranslateSpriteState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void TranslateSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	m_last_pos.MakeInvalid();
	if (m_dirty) {
		m_dirty = false;
		AtomicOP* aop = new TranslateSpriteAOP(*m_selection, pos - m_first_pos);
		EditAddRecordSJ::Instance()->Add(aop);
	}
}

bool TranslateSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selection->IsEmpty() || !m_last_pos.IsValid()) {
		return false;
	}

	m_dirty = true;
	Translate(pos - m_last_pos);
	m_last_pos = pos;

	return true;
}

bool TranslateSpriteState::OnDirectionKeyDown(int type)
{
	if (m_selection->IsEmpty()) return false;

	sm::vec2 offset;
	float len = (type & KEY_SHIFT) ? 10 : 1;
	switch (type & 0xf)
	{
	case KEY_LEFT:
		offset.Set(-len, 0);
		break;
	case KEY_RIGHT:
		offset.Set(len, 0);
		break;
	case KEY_DOWN:
		offset.Set(0, -len);
		break;
	case KEY_UP:
		offset.Set(0, len);
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