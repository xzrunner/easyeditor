#include "PreviewOP.h"

#include <easyanim.h>

#include <sprite2/UpdateParams.h>
#include <sprite2/AnimTreeCurr.h>

namespace eanim
{

PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimTreeCurr& curr)
	: ee::ZoomViewOP(wnd, stage, true, true, false)
	, m_curr(curr)
{
}

bool PreviewOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_curr.Start(s2::UpdateParams(), NULL);

	return false;
}

}