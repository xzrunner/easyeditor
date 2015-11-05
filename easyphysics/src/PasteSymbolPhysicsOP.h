#ifndef _EPHYSICS_PASTE_SYMBOL_PHYSICS_OP_H_
#define _EPHYSICS_PASTE_SYMBOL_PHYSICS_OP_H_

#include <drag2d.h>
#include <Box2D/Box2D.h>

namespace ephysics
{

class PhysicsPanelImpl;

class PasteSymbolPhysicsOP : public d2d::PasteSymbolOP
{
public:
	PasteSymbolPhysicsOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
		d2d::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, float* pScale = NULL);

	virtual bool OnMouseLeftDown(int x, int y);

	void setStatic(bool bStatic) { m_bStatic = bStatic; }

protected:
	bool m_bStatic;

private:
	b2World* m_world;

	float* m_pScale;

}; // PasteSymbolPhysicsOP

}

#endif // _EPHYSICS_PASTE_SYMBOL_PHYSICS_OP_H_