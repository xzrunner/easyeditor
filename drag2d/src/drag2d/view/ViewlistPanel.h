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

	void Remove(ISprite* sprite);
	void Insert(ISprite* sprite);
	void Reorder(const ISprite* sprite, bool up);
	void ReorderSelected(bool up);

	void OnSelected(int index);

	void Clear();

private:
	void InitLayout();

private:
	EditPanel* m_stage;
	MultiSpritesImpl* m_sprites_impl;
	PropertySettingPanel* m_property;

	ViewlistList* m_list;

	std::vector<ISprite*> m_sprites;

	int m_selected_idx;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_