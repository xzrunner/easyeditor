#ifndef _EASYEDITOR_LIBRARY_LIST_H_
#define _EASYEDITOR_LIBRARY_LIST_H_

#include "VerticalImageList.h"

#include <set>

namespace ee
{

class Symbol;
class LibraryPage;

class LibraryList : public VerticalImageList
{
public:
	LibraryList(LibraryPage* page, const std::string& filter 
		= std::string("symbol"), bool draggable = true);

	//
	//	interface VerticalImageList
	//
	virtual void OnListSelected(wxCommandEvent& event);
	virtual void Clear();
	virtual void Insert(ListItem* item, int idx = -1);
	virtual void Remove();
	virtual void Remove(int index);
	virtual void Swap(int i0, int i1);

	ListItem* GetItem(int index = -1) const;

	void ReloadTexture() const;

private:
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnKillFocus(wxFocusEvent& event);
	virtual void OnMouse(wxMouseEvent& event);

	void OnDragInit(wxMouseEvent& event);

private:
	std::set<int> m_selection_set;

}; // LibraryList

}

#endif // _EASYEDITOR_LIBRARY_LIST_H_