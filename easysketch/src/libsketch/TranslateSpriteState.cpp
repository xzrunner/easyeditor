#include "TranslateSpriteState.h"
#include "Sprite.h"

namespace libsketch
{

TranslateSpriteState::TranslateSpriteState(d2d::SpriteSelection* selection)
{
	m_selection = selection;
	m_selection->retain();
}

TranslateSpriteState::~TranslateSpriteState()
{
	m_selection->release();
}

void TranslateSpriteState::OnMousePress(const ivec2& pos)
{
	m_last_pos = pos;
}

void TranslateSpriteState::OnMouseRelease(const ivec2& pos)
{
	// todo history
}

void TranslateSpriteState::OnMouseMove(const ivec2& pos)
{
	if (m_selection->empty()) {
		return;
	}

	Translate(pos - m_last_pos);
	m_last_pos = pos;
}

void TranslateSpriteState::Translate(const ivec2& offset)
{
	m_selection->traverse(Visitor(offset));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
visit(d2d::Object* object, bool& bFetchNext)
{
	Sprite* sprite = static_cast<Sprite*>(object);
//	sprite->translate(m_offset);
	bFetchNext = true;
}

}