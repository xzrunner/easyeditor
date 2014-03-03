
#ifndef EBUILDER_SCALE_SIZE_PANEL_H
#define EBUILDER_SCALE_SIZE_PANEL_H

#include "BehaviorPanel.h"

namespace ebuilder
{
	class ScaleSize;

	class ScaleSizePanel : public BehaviorPanel
	{
	private:
		ScaleSizePanel(wxWindow* parent, ScaleSize* behavior);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend);

	private:
		wxSizer* createSetValueItem(wxWindow* extend);

		void onScaleChanged(wxSpinEvent& event);

	private:
		ScaleSize* m_behavior;

		friend class BehaviorFactory;

	}; // ScaleSizePanel
}

#endif // EBUILDER_SCALE_SIZE_PANEL_H
