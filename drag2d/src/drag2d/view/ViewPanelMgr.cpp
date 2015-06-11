#include "ViewPanelMgr.h"
#include "IViewPanel.h"

namespace d2d
{

void ViewPanelMgr::SelectSprite(ISprite* spr, IViewPanel* self)
{
	for (int i = 0, n = m_panels.size(); i < n; ++i) {
		if (m_panels[i] == self) {
			continue;
		}
		m_panels[i]->SelectSprite(spr);
	}
}

void ViewPanelMgr::AddPanel(IViewPanel* panel)
{
	m_panels.push_back(panel);
}

}