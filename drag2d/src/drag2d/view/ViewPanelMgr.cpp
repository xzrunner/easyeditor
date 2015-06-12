#include "ViewPanelMgr.h"
#include "IViewPanel.h"

namespace d2d
{

void ViewPanelMgr::AddPanel(IViewPanel* panel)
{
	m_panels.push_back(panel);
}

void ViewPanelMgr::SelectSprite(ISprite* spr, IViewPanel* expect)
{
	for (int i = 0, n = m_panels.size(); i < n; ++i) {
		if (m_panels[i] == expect) {
			continue;
		}
		m_panels[i]->SelectSprite(spr);
	}
}

void ViewPanelMgr::ReorderSprite(ISprite* spr, bool up, IViewPanel* expect)
{
	for (int i = 0, n = m_panels.size(); i < n; ++i) {
		if (m_panels[i] == expect) {
			continue;
		}
		m_panels[i]->ReorderSprite(spr, up);
	}
}

void ViewPanelMgr::InsertSprite(ISprite* spr, IViewPanel* expect)
{
	for (int i = 0, n = m_panels.size(); i < n; ++i) {
		if (m_panels[i] == expect) {
			continue;
		}
		m_panels[i]->InsertSprite(spr);
	}
}

void ViewPanelMgr::RemoveSprite(ISprite* spr, IViewPanel* expect)
{
	for (int i = 0, n = m_panels.size(); i < n; ++i) {
		if (m_panels[i] == expect) {
			continue;
		}
		m_panels[i]->RemoveSprite(spr);
	}
}

}