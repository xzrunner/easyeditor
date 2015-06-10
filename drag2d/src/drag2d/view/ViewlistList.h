#ifndef _DRAG2D_VIEW_LIST_H_
#define _DRAG2D_VIEW_LIST_H_

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
	void OnKeyDown(wxKeyEvent& event);

private:
	ViewlistPanel* m_panel;

	DECLARE_EVENT_TABLE()

}; // ViewlistList

}

#endif // _DRAG2D_VIEW_LIST_H_