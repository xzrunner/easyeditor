#ifndef _DRAG2D_VIEWLIST_PANEL_H_
#define _DRAG2D_VIEWLIST_PANEL_H_

#include <wx/wx.h>
#include <vector>

#include "message/Observer.h"

namespace d2d
{

class EditPanelImpl;
class MultiSpritesImpl;
class VerticalImageList;
class ISprite;
class ViewlistList;

class ViewlistPanel : public wxPanel, public Observer
{
public:
	ViewlistPanel(wxWindow* parent, EditPanelImpl* stage,
		MultiSpritesImpl* sprites_impl = NULL);
	virtual ~ViewlistPanel();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	void RemoveSelected();
	void ReorderSelected(bool up);

	void OnSelected(int index);
	void OnSelected(d2d::ISprite* spr);

	int GetSelectedIndex() const;

private:
	void InitLayout();

	int QuerySprIdx(const ISprite* spr) const;

	void Select(ISprite* spr, bool clear);
	void Reorder(const ISprite* sprite, bool up);
	void Insert(ISprite* sprite, int idx = -1);
	void Remove(ISprite* sprite);
	void Clear();

private:
	std::vector<Subject*> m_subjects;

	EditPanelImpl* m_stage;
	MultiSpritesImpl* m_sprites_impl;

	ViewlistList* m_list;

	std::vector<ISprite*> m_sprites;

	d2d::ISprite* m_selected_spr;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_