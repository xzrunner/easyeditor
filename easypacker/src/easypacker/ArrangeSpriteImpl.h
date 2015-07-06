#ifndef _EPACKER_ARRANGE_SPRITE_IMPL_H_
#define _EPACKER_ARRANGE_SPRITE_IMPL_H_

#include <drag2d.h>

namespace epacker
{

class StagePanel;

class ArrangeSpriteImpl : public d2d::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* editPanel, 
		d2d::PropertySettingPanel* propertyPanel);

	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseRightUp(int x, int y);

	virtual void OnDraw() const;

private:
	class FixCoordsVisitor : public d2d::IVisitor
	{
	public:
		virtual void Visit(d2d::Object* object, bool& bFetchNext);
	}; // FixCoordsVisitor

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}

#endif // _EPACKER_ARRANGE_SPRITE_IMPL_H_