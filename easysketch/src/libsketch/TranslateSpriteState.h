#ifndef _LIBSKETCH_TRANSLATE_SPRITE_STATE_H_
#define _LIBSKETCH_TRANSLATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class TranslateSpriteState : public IEditState
{
public:
	TranslateSpriteState(d2d::SpriteSelection* selection);
	virtual ~TranslateSpriteState();

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);	

private:
	void Translate(const ivec2& offset);

private:
	class Visitor : public d2d::IVisitor
	{
	public:
		Visitor(const ivec2& offset) : m_offset(offset) {}
		virtual void visit(d2d::Object* object, bool& bFetchNext);
	private:
		ivec2 m_offset;
	}; // Visitor

private:
	d2d::SpriteSelection* m_selection;

	ivec2 m_first_pos, m_last_pos;

}; // TranslateSpriteState

}

#endif // _LIBSKETCH_TRANSLATE_SPRITE_STATE_H_