#include "SelectSpritesOP.h"
#include "StageCanvas2D.h"

namespace eshader
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
	d2d::ViewPanelMgr* view_panel_mgr, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(stage_wnd, stage, spritesImpl, view_panel_mgr, callback)
{
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	static_cast<StageCanvas2D*>(m_stage->GetCanvas())->OnMousePressed(pos);

	return false;
}

}