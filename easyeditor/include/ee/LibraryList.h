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
	virtual void OnListSelected(wxCommandEvent& event) override;
	virtual void Clear() override;
	virtual void Insert(const ListItemPtr& item, int idx = -1) override;
	virtual void Remove() override;
	virtual void Remove(int index) override;
	virtual void Swap(int i0, int i1) override;

	ListItemPtr GetItem(int index = -1) const;

	void ReloadTexture() const;

private:
	virtual void OnKeyDown(wxKeyEvent& event) override;
	virtual void OnKillFocus(wxFocusEvent& event) override;
	virtual void OnMouse(wxMouseEvent& event) override;

	void OnDragInit(wxMouseEvent& event);

private:
	std::set<int> m_selection_set;

}; // LibraryList

}

#endif // _EASYEDITOR_LIBRARY_LIST_H_