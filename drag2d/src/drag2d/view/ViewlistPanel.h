#ifndef _DRAG2D_VIEWLIST_PANEL_H_
#define _DRAG2D_VIEWLIST_PANEL_H_

#include <wx/wx.h>
#include <vector>

#include "ISpriteViewPanel.h"

namespace d2d
{

class EditPanelImpl;
class MultiSpritesImpl;
class VerticalImageList;
class ISprite;
class ViewlistList;
class ViewPanelMgr;

class ViewlistPanel : public wxPanel, public ISpriteViewPanel
{
public:
	ViewlistPanel(wxWindow* parent, EditPanelImpl* stage,
		MultiSpritesImpl* sprites_impl = NULL, 
		ViewPanelMgr* view_panel_mgr = NULL);
	virtual ~ViewlistPanel();

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(ISprite* spr, bool clear);
	virtual void SelectMultiSprites(SpriteSelection* selection);
	virtual bool ReorderSprite(ISprite* spr, bool up);
	virtual bool InsertSprite(ISprite* spr);
	virtual bool RemoveSprite(ISprite* spr);
	virtual bool ClearAllSprite() { return false; }

	bool RemoveSelected();
	bool Remove(ISprite* sprite);
	bool Insert(ISprite* sprite);
	bool Reorder(const ISprite* sprite, bool up);
	bool ReorderSelected(bool up);

	void OnSelected(int index);
	void OnSelected(d2d::ISprite* spr);

	void Clear();

private:
	void InitLayout();

	int QuerySprIdx(const ISprite* spr) const;

private:
	EditPanelImpl* m_stage;
	MultiSpritesImpl* m_sprites_impl;

	ViewPanelMgr* m_view_panel_mgr;

	ViewlistList* m_list;

	std::vector<ISprite*> m_sprites;

	d2d::ISprite* m_selected_spr;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_