#pragma once

#include "widgets/VerticalImageList.h"

namespace d2d
{
	class ViewlistPanel;

	class ViewlistList : public VerticalImageList
	{
	public:
		ViewlistList(ViewlistPanel* panel);

		virtual void onListSelected(wxCommandEvent& event);
		virtual void onListDoubleClicked(wxCommandEvent& event);

	private:
		ViewlistPanel* m_panel;

	}; // ViewlistList
}

