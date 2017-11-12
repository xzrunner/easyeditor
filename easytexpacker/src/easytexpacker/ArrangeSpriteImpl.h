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

	virtual void OnMouseLeftUp(int x, int y) override;
	virtual void OnMouseRightDown(int x, int y) override;
	virtual void OnMouseRightUp(int x, int y) override;

	virtual void OnDraw(float cam_scale) const override;

private:
	class FixCoordsVisitor : public ee::Visitor<ee::Sprite>
	{
	public:
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;
	}; // FixCoordsVisitor

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}

#endif // _EASYTEXPACKER_ARRANGE_SPRITE_IMPL_H_