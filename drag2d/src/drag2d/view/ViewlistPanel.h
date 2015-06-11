#ifndef _DRAG2D_VIEWLIST_PANEL_H_
#define _DRAG2D_VIEWLIST_PANEL_H_

#include <wx/wx.h>
#include <vector>

#include "IViewPanel.h"

namespace d2d
{

class EditPanel;
class MultiSpritesImpl;
class PropertySettingPanel;
class VerticalImageList;
class ISprite;
class ViewlistList;
class ViewPanelMgr;

class ViewlistPanel : public wxPanel, public IViewPanel
{
public:
	ViewlistPanel(wxWindow* parent, EditPanel* stage,
		MultiSpritesImpl* sprites_impl = NULL, 
		PropertySettingPanel* property = NULL,
		ViewPanelMgr* view_panel_mgr = NULL);
	virtual ~ViewlistPanel();

	//
	//	interface IViewPanel
	//
	virtual void SelectSprite(ISprite* spr);
	virtual void ReorderSprite(ISprite* spr, bool up);

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
	PropertySettingPanel* m_property;

	ViewPanelMgr* m_view_panel_mgr;

	ViewlistList* m_list;

	std::vector<ISprite*> m_sprites;

	d2d::ISprite* m_selected_spr;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_