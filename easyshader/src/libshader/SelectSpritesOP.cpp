#include "SelectSpritesOP.h"
#include "StageCanvas2D.h"

#include <ee/EditPanelImpl.h>

namespace eshader
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
								 ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(stage_wnd, stage, sprites_impl, callback)
{
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	static_cast<StageCanvas2D*>(m_stage->GetCanvas())->OnMousePressed(pos);

	return false;
}

}