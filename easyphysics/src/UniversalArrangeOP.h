#ifndef _EPHYSICS_UNIVERSAL_ARRANGE_OP_H_
#define _EPHYSICS_UNIVERSAL_ARRANGE_OP_H_

#include <drag2d.h>
#include <Box2D/Box2D.h>

namespace ephysics
{

class d2d::MultiSpritesImpl;
class DragPhysicsOP;
class PropertySettingPanel;
class AbstractEditCMPT;

class UniversalArrangeOP : public d2d::ZoomViewOP
{
public:
	UniversalArrangeOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl,
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);
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
	d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>* m_noPhysics;

	d2d::AbstractEditOP* m_editOP;

}; // UniversalArrangeOP

}

#endif // _EPHYSICS_UNIVERSAL_ARRANGE_OP_H_