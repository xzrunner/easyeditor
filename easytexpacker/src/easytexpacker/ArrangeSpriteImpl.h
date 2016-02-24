#ifndef _EASYTEXPACKER_ARRANGE_SPRITE_IMPL_H_
#define _EASYTEXPACKER_ARRANGE_SPRITE_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>
#include <ee/Visitor.h>

namespace etexpacker
{

class StagePanel;

class ArrangeSpriteImpl : public ee::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* editPanel, 
		ee::PropertySettingPanel* property);

	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseRightUp(int x, int y);

	virtual void OnDraw() const;

private:
	class FixCoordsVisitor : public ee::Visitor
	{
	public:
		virtual void Visit(ee::Object* object, bool& next);
	}; // FixCoordsVisitor

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}

#endif // _EASYTEXPACKER_ARRANGE_SPRITE_IMPL_H_