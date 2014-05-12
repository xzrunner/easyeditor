
#ifndef EBUILDER_BEHAVIOR_PANEL_H
#define EBUILDER_BEHAVIOR_PANEL_H

#include "CollapsiblePane.h"

#include <drag2d.h>

namespace ebuilder
{
	class BehaviorPanel : public CollapsiblePane
	{
	public:
		BehaviorPanel(wxWindow* parent, const wxString& name);

		virtual BehaviorPanel* clone() {
			return NULL;
		}

		virtual void update(const d2d::LibraryList& list) {}

		void createPanel(wxWindow* parent);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend) = 0;

	protected:
		static const wxString TIP_DRAG_BEHAVIOR;

		static const wxSize TIP_SIZE;

	}; // BehaviorPanel
}

#endif // EBUILDER_BEHAVIOR_PANEL_H
