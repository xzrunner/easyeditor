
#ifndef EBUILDER_CHANGE_IMAGE_PANEL_H
#define EBUILDER_CHANGE_IMAGE_PANEL_H

#include "BehaviorPanel.h"

namespace ebuilder
{
	class ChangeImage;

	class ChangeImagePanel : public BehaviorPanel
	{
	public:
		virtual void update(const d2d::LibraryList& list);

	private:
		ChangeImagePanel(wxWindow* parent, d2d::LibraryList* mediaList, 
			ChangeImage* behavior);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend);

	private:
		wxSizer* createChooseListItem(wxWindow* extend);

		void onChoiceChanged(wxCommandEvent& event);

		void initChoice();

	private:
		d2d::LibraryList* m_list;

		ChangeImage* m_behavior;

		wxChoice* m_choice;

		friend class BehaviorFactory;

	}; // ChangeImagePanel
}

#endif // EBUILDER_CHANGE_IMAGE_PANEL_H
