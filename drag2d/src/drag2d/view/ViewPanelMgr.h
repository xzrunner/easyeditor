#ifndef _DRAG2D_VIEW_PANEL_MGR_H_
#define _DRAG2D_VIEW_PANEL_MGR_H_

#include <vector>

namespace d2d
{

class ISprite;
class IViewPanel;

class ViewPanelMgr
{
public:

	void AddPanel(IViewPanel* panel);

	void SelectSprite(ISprite* spr, IViewPanel* self);

	void ReorderSprite(ISprite* spr, bool up, IViewPanel* self);

private:
	std::vector<IViewPanel*> m_panels;

}; // ViewPanelMgr

}

#endif // _DRAG2D_VIEW_PANEL_MGR_H_