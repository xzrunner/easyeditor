#include "PreviewOP.h"

#include <easyanim.h>

namespace eanim
{

#ifdef S2_ANIM_CURR_V0
PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr& curr)
#elif defined S2_ANIM_CURR_V1
PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr2& curr)
#elif defined S2_ANIM_CURR_V2
PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr3& curr)
#endif
	: ee::ZoomViewOP(wnd, stage, true, true, false)
	, m_curr(curr)
{
}

bool PreviewOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_curr.Start();

	return false;
}

}