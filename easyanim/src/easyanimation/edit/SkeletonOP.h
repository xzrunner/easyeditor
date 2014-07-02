#ifndef EANIM_SKELETON_OP_H
#define EANIM_SKELETON_OP_H

#include "SelectSpritesOP.h"
#include "KeyDownHandler.h"

namespace eanim
{

class StagePanel;
class Joint;

class SkeletonOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	SkeletonOP(StagePanel* stage);

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onPopMenuSelected(int type);

	virtual bool onDraw(const d2d::Screen& scr) const;

protected:
	virtual void translateSprite(const d2d::Vector& delta);
	virtual void rotateSprite(const d2d::Vector& dst);
	virtual void scaleSprite(const d2d::Vector& currPos);

	virtual void setRightPopupMenu(wxMenu& menu);
	virtual bool isOffsetEnable() const { return false; }

private:
	KeyDownHandler m_keyDownHandler;

	StagePanel* m_stage;

	Joint* m_selectedJoint;

}; // SkeletonOP

}

#endif // EANIM_SKELETON_OP_H