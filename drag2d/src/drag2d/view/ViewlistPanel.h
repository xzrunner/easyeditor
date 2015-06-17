#ifndef _DRAG2D_VIEWLIST_PANEL_H_
#define _DRAG2D_VIEWLIST_PANEL_H_

#include <wx/wx.h>
#include <vector>

#include "ISpriteViewPanel.h"

namespace d2d
{

class EditPanel;
class MultiSpritesImpl;
class VerticalImageList;
class ISprite;
class ViewlistList;
class ViewPanelMgr;

class ViewlistPanel : public wxPanel, public ISpriteViewPanel
{
public:
	ViewlistPanel(wxWindow* parent, EditPanel* stage,
		MultiSpritesImpl* sprites_impl = NULL, 
		ViewPanelMgr* view_panel_mgr = NULL);
	virtual ~ViewlistPanel();

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(ISprite* spr);
	virtual void SelectMultiSprites(SpriteSelection* selection);
	virtual void ReorderSprite(ISprite* spr, bool up);
	virtual void InsertSprite(ISprite* spr);
	virtual void RemoveSprite(ISprite* spr);

	void Remove(ISprite* sprite);
	void Insert(ISprite* sprite);
	void Reorder(const ISprite* sprite, bool up);
	void ReorderSelected(bool up);

	void OnSelected(int index);
	void OnSelected(d2d::ISprite* spr);

	void Clear();

private:
	void InitLayout();

	int QuerySprIdx(const ISprite* spr) const;

private:
	EditPanel* m_stage;
	MultiSpritesImpl* m_sprites_impl;

	ViewPanelMgr* m_view_panel_mgr;

	ViewlistList* m_list;

	std::vector<ISprite*> m_sprites;

	d2d::ISprite* m_selected_spr;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_