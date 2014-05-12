
#ifndef EBUILDER_MOUSE_BUTTON_PANEL_H
#define EBUILDER_MOUSE_BUTTON_PANEL_H

#include "BehaviorPanel.h"

namespace ebuilder
{
	class MouseButton;
	class Actor;

	class MouseButtonPanel : public BehaviorPanel
	{
	private:
		MouseButtonPanel(wxWindow* parent,
			MouseButton* behavior);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend);

	private:
		wxSizer* createMouseStateListItem(wxWindow* extend);
		wxSizer* createSatisfyPanel(wxWindow* extend);
		wxSizer* createNotSatisfyPanel(wxWindow* extend);

		void onChoiceChanged(wxCommandEvent& event);

	private:
		class DragTarget : public wxTextDropTarget
		{
		public:
			DragTarget(wxWindow* parent, Actor* actor,
				MouseButton* behavior, bool bSatisfy);

			virtual bool OnDropText(wxCoord x, wxCoord y, 
				const wxString& data);

		private:
			wxWindow* m_parent;

			Actor* m_actor;

			MouseButton* m_behavior;

			bool m_bSatisfy;

		}; // DragTarget

	private:
		MouseButton* m_behavior;

		wxWindow* m_extendSatisfy;
		wxWindow* m_extendNotSatisfy;

		friend class BehaviorFactory;

	}; // MouseButtonPanel
}

#endif // EBUILDER_MOUSE_BUTTON_PANEL_H
