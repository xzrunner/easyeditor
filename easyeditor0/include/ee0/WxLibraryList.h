#pragma once

#include "ee0/WxImageVList.h"

#include <set>

namespace ee0
{

class WxLibraryList : public WxImageVList
{
public:
	WxLibraryList(wxWindow* parent, const std::string& name);

	virtual void OnListSelected(wxCommandEvent& event) override;
	virtual void Clear() override;
	virtual void Insert(const std::shared_ptr<WxLibraryItem>& item, int idx = -1) override;
	virtual void Remove() override;
	virtual void Remove(int index) override;
	virtual void Swap(int i0, int i1) override;

	std::shared_ptr<WxLibraryItem> GetItem(int idx = -1) const;

protected:
	virtual void OnKeyDown(wxKeyEvent& event) override;
	virtual void OnKillFocus(wxFocusEvent& event) override;
	virtual void OnMouse(wxMouseEvent& event) override;

private:
	std::set<int> m_selection_set;

}; // WxLibraryList

}