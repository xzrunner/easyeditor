#include "PreviewOP.h"

#include <easyanim.h>

namespace eanim
{

PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr& curr)
	: ee::ZoomViewOP(wnd, stage, true, true, false)
	, m_curr(curr)
{
}

bool PreviewOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_curr.Start(NULL);

	return false;
}

}