#ifndef _EPHYSICS_UNIVERSAL_ARRANGE_OP_H_
#define _EPHYSICS_UNIVERSAL_ARRANGE_OP_H_


#include <Box2D/Box2D.h>

namespace ephysics
{

class ee::MultiSpritesImpl;
class DragPhysicsOP;
class PropertySettingPanel;
class EditCMPT;

class UniversalArrangeOP : public ee::ZoomViewOP
{
public:
	UniversalArrangeOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* spritesImpl,
		ee::PropertySettingPanel* property, ee::EditCMPT* callback = NULL);
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
	ee::ArrangeSpriteOP<ee::SelectSpritesOP>* m_noPhysics;

	ee::EditOP* m_editop;

}; // UniversalArrangeOP

}

#endif // _EPHYSICS_UNIVERSAL_ARRANGE_OP_H_