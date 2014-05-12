
#ifndef EBUILDER_ROTATE_PANEL_H
#define EBUILDER_ROTATE_PANEL_H

#include "BehaviorPanel.h"

namespace ebuilder
{
	class Rotate;

	class RotatePanel : public BehaviorPanel
	{
	private:
		RotatePanel(wxWindow* parent, ebuilder::Rotate* behavior);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend);

	private:
		void onPGChange(wxPropertyGridEvent& event);

	private:
		ebuilder::Rotate* m_behavior;

		wxPropertyGrid* m_pg;

		friend class BehaviorFactory;

	}; // RotatePanel
}

#endif // EBUILDER_ROTATE_PANEL_H
