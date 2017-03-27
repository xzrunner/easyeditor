#include "PreviewEditOP.h"

#include <ee/Visitor.h>
#include <ee/Sprite.h>

#include <easyparticle3d.h>
#include <easyanim.h>

#include <sprite2/UpdateParams.h>

namespace ecomplex
{

PreviewEditOP::PreviewEditOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							 const std::vector<ee::Sprite*>& sprs)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_sprs(sprs)
{
}

bool PreviewEditOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->OnMessage(s2::UpdateParams(), s2::MSG_START);
	}

	return false;
}

}