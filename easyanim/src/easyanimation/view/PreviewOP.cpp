#include "PreviewOP.h"

#include <easyanim.h>

namespace eanim
{

#ifdef S2_ANIM_CURR_OLD
PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr& curr)
#else
PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr2& curr)
#endif // S2_ANIM_CURR_OLD
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