#ifndef _EASYEDITOR_VIEW_LIST_H_
#define _EASYEDITOR_VIEW_LIST_H_

#include "VerticalImageList.h"
#include "Observer.h"
#include "Sprite.h"

namespace ee
{

class ViewlistListImpl;
class SpriteSelection;

class ViewlistList : public VerticalImageList, public Observer
{
public:
	ViewlistList(wxWindow* parent);
	virtual ~ViewlistList();

	virtual void OnListSelected(wxCommandEvent& event) override;
	virtual void OnListDoubleClicked(wxCommandEvent& event) override;

	void SetImpl(ViewlistListImpl* impl);

	void Clear();
	void Insert(const SprPtr& spr, int idx = -1);

	void OnSelected(int idx, bool clear);

	SprPtr QuerySprite(int idx);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

	virtual void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const override;

	virtual void OnKeyDown(wxKeyEvent& event) override;
	virtual void OnMouse(wxMouseEvent& event) override;

private:
	int GetSelectedIndex() const;

	void OnSelected(const SprPtr& spr, bool clear);

	int QuerySprIdx(const SprConstPtr& spr) const;

	void ReorderSelected(bool up);

	void Select(const SprPtr& spr, bool clear);
	void SelectSet(SpriteSelection* set);
	void Reorder(const SprConstPtr& spr, bool up);
	void ReorderMost(const SprConstPtr& spr, bool up);
	void Remove(const SprPtr& spr);
	void RemoveSelected();

private:
	ViewlistListImpl* m_impl;

	std::vector<SprPtr> m_sprs;

	SprPtr m_selected_spr;

}; // ViewlistList

}

#endif // _EASYEDITOR_VIEW_LIST_H_