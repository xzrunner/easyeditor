#ifndef _DRAG2D_VIEW_LIST_H_
#define _DRAG2D_VIEW_LIST_H_

#include "KeysState.h"

#include "widgets/VerticalImageList.h"

namespace d2d
{

class ViewlistPanel;

class ViewlistList : public VerticalImageList
{
public:
	ViewlistList(ViewlistPanel* panel);

	virtual void OnListSelected(wxCommandEvent& event);
	virtual void OnListDoubleClicked(wxCommandEvent& event);

	bool GetKeyState(int key) const {
		return m_keys_state.GetKeyState(key);
	}

private:
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

private:
	ViewlistPanel* m_panel;

	KeysState m_keys_state;

	DECLARE_EVENT_TABLE()

}; // ViewlistList

}

#endif // _DRAG2D_VIEW_LIST_H_