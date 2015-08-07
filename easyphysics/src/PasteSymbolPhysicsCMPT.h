#ifndef _EPHYSICS_PASTE_SYMBOL_PHYSICS_CMPT_H_
#define _EPHYSICS_PASTE_SYMBOL_PHYSICS_CMPT_H_

#include <drag2d.h>
#include <Box2D/Box2D.h>

namespace ephysics
{

class PhysicsPanelImpl;

class PasteSymbolPhysicsCMPT : public d2d::AbstractEditCMPT
{
public:
	PasteSymbolPhysicsCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiSpritesImpl* spritesImpl, d2d::LibraryPanel* libraryPanel,
		PhysicsPanelImpl* physicsImpl, b2World* world, b2World* worldReverse,
		float* pScale = NULL, d2d::PasteSymbolRandomWidget* randomWidget = NULL);

protected:
	virtual wxSizer* initLayout();

private:
	void onChangeGravity(wxCommandEvent& event);

	void onChangeStaticType(wxCommandEvent& event);

private:
	wxRadioBox* m_gravityChoice;

	b2World *m_world, *m_worldReverse;

}; // PasteSymbolPhysicsCMPT

}

#endif // _EPHYSICS_PASTE_SYMBOL_PHYSICS_CMPT_H_