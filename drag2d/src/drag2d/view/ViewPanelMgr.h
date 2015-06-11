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

	void SelectSprite(ISprite* spr, IViewPanel* self);

	void AddPanel(IViewPanel* panel);

private:
	std::vector<IViewPanel*> m_panels;

}; // ViewPanelMgr

}

#endif // _DRAG2D_VIEW_PANEL_MGR_H_