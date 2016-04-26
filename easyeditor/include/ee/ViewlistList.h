#ifndef _EASYEDITOR_VIEW_LIST_H_
#define _EASYEDITOR_VIEW_LIST_H_

#include "VerticalImageList.h"
#include "Observer.h"

namespace ee
{

class Sprite;
class ViewlistListImpl;
class SpriteSelection;

class ViewlistList : public VerticalImageList, public Observer
{
public:
	ViewlistList(wxWindow* parent);
	virtual ~ViewlistList();

	virtual void OnListSelected(wxCommandEvent& event);
	virtual void OnListDoubleClicked(wxCommandEvent& event);

	void SetImpl(ViewlistListImpl* impl);

	void Clear();
	void Insert(Sprite* sprite, int idx = -1);

	void OnSelected(int idx, bool clear);

	Sprite* QuerySprite(int idx);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

	virtual void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const;

	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnMouse(wxMouseEvent& event);

private:
	int GetSelectedIndex() const;

	void OnSelected(Sprite* spr, bool clear);

	int QuerySprIdx(const Sprite* spr) const;

	void ReorderSelected(bool up);

	void Select(Sprite* spr, bool clear);
	void SelectSet(SpriteSelection* set);
	void Reorder(const Sprite* sprite, bool up);
	void ReorderMost(const Sprite* sprite, bool up);
	void Remove(Sprite* sprite);
	void RemoveSelected();

private:
	ViewlistListImpl* m_impl;

	std::vector<Sprite*> m_sprites;

	Sprite* m_selected_spr;

}; // ViewlistList

}

#endif // _EASYEDITOR_VIEW_LIST_H_