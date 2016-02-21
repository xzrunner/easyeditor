#ifndef _EASYEDITOR_PASTE_SYMBOL_PHYSICS_OP_H_
#define _EASYEDITOR_PASTE_SYMBOL_PHYSICS_OP_H_

#include "PasteSymbolOP.h"

#include <Box2D/Box2D.h>

namespace ee
{

class PhysicsPanelImpl;

class PasteSymbolPhysicsOP : public PasteSymbolOP
{
public:
	PasteSymbolPhysicsOP(wxWindow* stage_wnd, EditPanelImpl* stage, 
		LibraryPanel* library, PhysicsPanelImpl* physicsImpl, float* pScale = NULL);

	virtual bool OnMouseLeftDown(int x, int y);

	void setStatic(bool bStatic) { m_bStatic = bStatic; }

protected:
	bool m_bStatic;

private:
	b2World* m_world;

	float* m_pScale;

}; // PasteSymbolPhysicsOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_PHYSICS_OP_H_