#ifndef _EASYEDITOR_VIEW_LIST_H_
#define _EASYEDITOR_VIEW_LIST_H_

#include "VerticalImageList.h"
#include "Observer.h"

namespace ee
{

class Sprite;
class ViewlistListImpl;

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

	void OnSelected(int idx);

	Sprite* QuerySprite(int idx);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	virtual void OnKeyDown(wxKeyEvent& event);

	int GetSelectedIndex() const;

	void OnSelected(Sprite* spr);

	int QuerySprIdx(const Sprite* spr) const;

	void ReorderSelected(bool up);

	void Select(Sprite* spr, bool clear);
	void Reorder(const Sprite* sprite, bool up);
	void Remove(Sprite* sprite);
	void RemoveSelected();

private:
	ViewlistListImpl* m_impl;

	std::vector<Sprite*> m_sprites;

	Sprite* m_selected_spr;

}; // ViewlistList

}

#endif // _EASYEDITOR_VIEW_LIST_H_