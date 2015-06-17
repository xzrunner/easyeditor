#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"

namespace emap
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library,
					   d2d::PropertySettingPanel* property,
					   d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this)
{
	m_paste_op = new d2d::PasteSymbolOP(this, this, library);
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property, view_panel_mgr);

	m_editOP = m_paste_op;
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
}

void StagePanel::clear()
{
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
	if (!m_editOP->IsEmpty()) {
		return;
	}

	if (m_editOP == m_arrange_op) {
		m_editOP = m_paste_op;
	} else {
		m_editOP = m_arrange_op;
	}
}

}