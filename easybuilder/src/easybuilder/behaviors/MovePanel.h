
#ifndef EBUILDER_MOVE_PANEL_H
#define EBUILDER_MOVE_PANEL_H

#include "BehaviorPanel.h"

namespace ebuilder
{
	class Move;

	class MovePanel : public BehaviorPanel
	{
	private:
		MovePanel(wxWindow* parent, ebuilder::Move* behavior);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend);

	private:
		void onPGChange(wxPropertyGridEvent& event);

	private:
		ebuilder::Move* m_behavior;

		wxPropertyGrid* m_pg;

		friend class BehaviorFactory;

	}; // MovePanel
}

#endif // EBUILDER_MOVE_PANEL_H
