#ifndef _DRAG2D_VIEWLIST_PANEL_H_
#define _DRAG2D_VIEWLIST_PANEL_H_

#include <wx/wx.h>
#include <vector>

namespace d2d
{

class EditPanel;
class MultiSpritesImpl;
class PropertySettingPanel;
class VerticalImageList;
class ISprite;
class ViewlistList;

class ViewlistPanel : public wxPanel
{
public:
	ViewlistPanel(wxWindow* parent, EditPanel* stage,
		MultiSpritesImpl* sprites_impl = NULL, 
		PropertySettingPanel* property = NULL);

	void remove(ISprite* sprite);
	void insert(ISprite* sprite);
	void reorder(const ISprite* sprite, bool up);

	void onSelected(int index);

	void clear();

private:
	void initLayout();

private:
	EditPanel* m_stage;
	MultiSpritesImpl* m_sprites_impl;
	PropertySettingPanel* m_property;

	ViewlistList* m_list;

	std::vector<ISprite*> m_sprites;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_