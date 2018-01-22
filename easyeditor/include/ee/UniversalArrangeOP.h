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

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	void addPhysicsEditOP(b2World* world, b2Body* ground);

private:
	struct PhysicsOP
	{
		std::shared_ptr<DragPhysicsOP> editop = nullptr;
		b2World* world;
	};

private:
	std::vector<PhysicsOP> m_physics;
	std::shared_ptr<ArrangeSpriteOP<SelectSpritesOP>> m_no_physics;

	std::shared_ptr<EditOP> m_editop = nullptr;

}; // UniversalArrangeOP

}

#endif // _EASYEDITOR_UNIVERSAL_ARRANGE_OP_H_