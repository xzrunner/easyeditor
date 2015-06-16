#include "PreviewEditOP.h"

#include <easyparticle3d.h>

namespace ecomplex
{

PreviewEditOP::PreviewEditOP(d2d::EditPanel* stage, 
							 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::ZoomViewOP(stage, true)
	, m_sprites(sprites)
{
}

bool PreviewEditOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (const eparticle3d::Sprite* particle = dynamic_cast<const eparticle3d::Sprite*>(m_sprites[i])) {
			eparticle3d::Symbol& symbol = const_cast<eparticle3d::Symbol&>(particle->getSymbol());
			symbol.Start();
		}
	}

	return false;
}

}