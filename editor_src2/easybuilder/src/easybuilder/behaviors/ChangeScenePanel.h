
#ifndef EBUILDER_CHANGE_SCENE_PANEL_H
#define EBUILDER_CHANGE_SCENE_PANEL_H

#include "BehaviorPanel.h"

namespace ebuilder
{
	class ChangeScene;

	class ChangeScenePanel : public BehaviorPanel
	{
	public:
		virtual void update(const d2d::LibraryList& list);

	private:
		ChangeScenePanel(wxWindow* parent, d2d::LibraryList* sceneList,
			ChangeScene* behavior);

	protected:
		virtual wxSizer* createExtend(wxWindow* extend);

	private:
		wxSizer* createChooseListItem(wxWindow* extend);

		void onChoiceChanged(wxCommandEvent& event);

		void initChoice();

	private:
		d2d::LibraryList* m_list;

		ChangeScene* m_behavior;

		wxChoice* m_choice;

		friend class BehaviorFactory;

	}; // ChangeScenePanel
}

#endif // EBUILDER_CHANGE_SCENE_PANEL_H
