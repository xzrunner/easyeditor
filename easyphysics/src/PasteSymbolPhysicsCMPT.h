#ifndef _EPHYSICS_PASTE_SYMBOL_PHYSICS_CMPT_H_
#define _EPHYSICS_PASTE_SYMBOL_PHYSICS_CMPT_H_


#include <Box2D/Box2D.h>

namespace ephysics
{

class PhysicsPanelImpl;

class PasteSymbolPhysicsCMPT : public ee::EditCMPT
{
public:
	PasteSymbolPhysicsCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* spritesImpl, ee::LibraryPanel* libraryPanel,
		PhysicsPanelImpl* physicsImpl, b2World* world, b2World* worldReverse,
		float* pScale = NULL, ee::PasteSymbolRandomWidget* randomWidget = NULL);

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

#endif // _EPHYSICS_PASTE_SYMBOL_PHYSICS_CMPT_H_