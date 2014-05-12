#pragma once

#include "AbstractEditCMPT.h"

#include <Box2D/Box2D.h>

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;
	class PhysicsPanelImpl;
	class PasteSymbolRandomWidget;

	class PasteSymbolPhysicsCMPT : public AbstractEditCMPT
	{
	public:
		PasteSymbolPhysicsCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiSpritesImpl* spritesImpl, LibraryPanel* libraryPanel,
			PhysicsPanelImpl* physicsImpl, b2World* world, b2World* worldReverse,
			float* pScale = NULL, PasteSymbolRandomWidget* randomWidget = NULL);

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

