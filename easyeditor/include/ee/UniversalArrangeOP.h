#ifndef _EASYEDITOR_UNIVERSAL_ARRANGE_OP_H_
#define _EASYEDITOR_UNIVERSAL_ARRANGE_OP_H_

#include "ZoomViewOP.h"
#include "ArrangeSpriteOP.h"
#include "SelectSpritesOP.h"

#include <Box2D/Box2D.h>

#include <vector>

namespace ee
{

class MultiSpritesImpl;
class DragPhysicsOP;
class PropertySettingPanel;
class EditCMPT;

class UniversalArrangeOP : public ZoomViewOP
{
public:
	UniversalArrangeOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl,
		PropertySettingPanel* property, EditCMPT* callback = NULL);
	virtual ~UniversalArrangeOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void addPhysicsEditOP(b2World* world, b2Body* ground);

private:
	struct PhysicsOP
	{
		DragPhysicsOP* editOP;
		b2World* world;
	};

private:
	std::vector<PhysicsOP> m_physics;
	ArrangeSpriteOP<SelectSpritesOP>* m_noPhysics;

	EditOP* m_editop;

}; // UniversalArrangeOP

}

#endif // _EASYEDITOR_UNIVERSAL_ARRANGE_OP_H_