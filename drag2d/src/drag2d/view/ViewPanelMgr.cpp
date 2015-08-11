#include "ViewPanelMgr.h"
#include "ISpriteViewPanel.h"
#include "IShapeViewPanel.h"
#include "ViewlistPanel.h"

namespace d2d
{

ViewPanelMgr::ViewPanelMgr() 
	: m_locked(false) 
	, m_viewlist(NULL)
{
}

void ViewPanelMgr::AddSpritePanel(ISpriteViewPanel* panel)
{
	m_spr_panels.push_back(panel);
}

void ViewPanelMgr::AddShapePanel(IShapeViewPanel* panel)
{
	m_shape_panels.push_back(panel);
}

void ViewPanelMgr::SelectSprite(ISprite* spr, bool clear, ISpriteViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_spr_panels.size(); i < n; ++i) {
		if (m_spr_panels[i] == expect) {
			continue;
		}
		m_spr_panels[i]->SelectSprite(spr, clear);
	}

	m_locked = false;
}

void ViewPanelMgr::SelectMultiSprites(SpriteSelection* selection, ISpriteViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_spr_panels.size(); i < n; ++i) {
		if (m_spr_panels[i] == expect) {
			continue;
		}
		m_spr_panels[i]->SelectMultiSprites(selection);
	}

	m_locked = false;
}

void ViewPanelMgr::ReorderSprite(ISprite* spr, bool up, ISpriteViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_spr_panels.size(); i < n; ++i) {
		if (m_spr_panels[i] == expect) {
			continue;
		}
		m_spr_panels[i]->ReorderSprite(spr, up);
	}

	m_locked = false;
}

void ViewPanelMgr::InsertSprite(ISprite* spr, ISpriteViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_spr_panels.size(); i < n; ++i) {
		if (m_spr_panels[i] == expect) {
			continue;
		}
		m_spr_panels[i]->InsertSprite(spr);
	}

	m_locked = false;
}

void ViewPanelMgr::RemoveSprite(ISprite* spr, ISpriteViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_spr_panels.size(); i < n; ++i) {
		if (m_spr_panels[i] == expect) {
			continue;
		}
		m_spr_panels[i]->RemoveSprite(spr);
	}

	m_locked = false;
}

void ViewPanelMgr::SelectShape(IShape* shape, IShapeViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_shape_panels.size(); i < n; ++i) {
		if (m_shape_panels[i] == expect) {
			continue;
		}
		m_shape_panels[i]->SelectShape(shape);
	}

	m_locked = false;
}

void ViewPanelMgr::SelectMultiShapes(ShapeSelection* selection, IShapeViewPanel* expect)
{
	if (m_locked) {
		return;
	} else {
		m_locked = true;
	}

	for (int i = 0, n = m_shape_panels.size(); i < n; ++i) {
		if (m_shape_panels[i] == expect) {
			continue;
		}
		m_shape_panels[i]->SelectMultiShapes(selection);
	}

	m_locked = false;
}

int ViewPanelMgr::GetSelection() const
{
	if (m_viewlist) {
		return m_viewlist->GetSelectedIndex();
	} else {
		return INT_MAX;
	}
}

void ViewPanelMgr::SetViewlistPanel(ViewlistPanel* viewlist)
{
	m_viewlist = viewlist;
}

}