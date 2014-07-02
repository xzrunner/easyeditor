#pragma once

#include "ZoomViewOP.h"
#include "ArrangeSpriteOP.h"
#include "SelectSpritesOP.h"

#include <Box2D/Box2D.h>

namespace d2d
{
	class MultiSpritesImpl;
	class DragPhysicsOP;
	class PropertySettingPanel;
	class AbstractEditCMPT;

	class UniversalArrangeOP : public ZoomViewOP
	{
	public:
		UniversalArrangeOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
			PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback = NULL);
		virtual ~UniversalArrangeOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw(const Screen& scr) const;
		virtual bool clear();

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

		AbstractEditOP* m_editOP;

	}; // UniversalArrangeOP
}

