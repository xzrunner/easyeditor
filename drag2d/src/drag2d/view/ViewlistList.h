#ifndef _DRAG2D_VIEW_LIST_H_
#define _DRAG2D_VIEW_LIST_H_

#include "widgets/VerticalImageList.h"
#include "message/Observer.h"

namespace d2d
{

class ISprite;
class ViewlistListImpl;

class ViewlistList : public VerticalImageList, public Observer
{
public:
	ViewlistList(wxWindow* parent);
	virtual ~ViewlistList();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	virtual void OnListSelected(wxCommandEvent& event);
	virtual void OnListDoubleClicked(wxCommandEvent& event);

	void SetImpl(ViewlistListImpl* impl);

	void Clear();
	void Insert(ISprite* sprite, int idx = -1);

	void OnSelected(int idx);

	d2d::ISprite* QuerySprite(int idx);

private:
	virtual void OnKeyDown(wxKeyEvent& event);

	int GetSelectedIndex() const;

	void OnSelected(d2d::ISprite* spr);

	int QuerySprIdx(const ISprite* spr) const;

	void ReorderSelected(bool up);

	void Select(ISprite* spr, bool clear);
	void Reorder(const ISprite* sprite, bool up);
	void Remove(ISprite* sprite);
	void RemoveSelected();

private:
	ViewlistListImpl* m_impl;

	std::vector<ISprite*> m_sprites;

	d2d::ISprite* m_selected_spr;

	std::vector<Subject*> m_subjects;

}; // ViewlistList

}

#endif // _DRAG2D_VIEW_LIST_H_