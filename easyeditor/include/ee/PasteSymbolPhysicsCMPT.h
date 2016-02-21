#ifndef _EASYEDITOR_PASTE_SYMBOL_PHYSICS_CMPT_H_
#define _EASYEDITOR_PASTE_SYMBOL_PHYSICS_CMPT_H_

#include "EditCMPT.h"

#include <Box2D/Box2D.h>

namespace ee
{

class PhysicsPanelImpl;
class MultiSpritesImpl;
class LibraryPanel;
class PasteSymbolRandomWidget;

class PasteSymbolPhysicsCMPT : public EditCMPT
{
public:
	PasteSymbolPhysicsCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* sprites_impl, LibraryPanel* library,
		PhysicsPanelImpl* physicsImpl, b2World* world, b2World* worldReverse,
		float* pScale = NULL, PasteSymbolRandomWidget* randomWidget = NULL);

protected:
	virtual wxSizer* InitLayout();

private:
	void onChangeGravity(wxCommandEvent& event);

	void onChangeStaticType(wxCommandEvent& event);

private:
	wxRadioBox* m_gravityChoice;

	b2World *m_world, *m_worldReverse;

}; // PasteSymbolPhysicsCMPT

}

#endif // _EASYEDITOR_PASTE_SYMBOL_PHYSICS_CMPT_H_