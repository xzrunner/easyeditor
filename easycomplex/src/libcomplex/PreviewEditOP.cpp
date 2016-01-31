#include "PreviewEditOP.h"

#include <easyparticle3d.h>

namespace ecomplex
{

PreviewEditOP::PreviewEditOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							 const std::vector<const ee::Sprite*>& sprites)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_sprites(sprites)
{
}

bool PreviewEditOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (const eparticle3d::Sprite* particle = dynamic_cast<const eparticle3d::Sprite*>(m_sprites[i])) {
			const_cast<eparticle3d::Sprite*>(particle)->Start();
		}
	}

	return false;
}

}