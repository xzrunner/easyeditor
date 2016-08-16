#include "PreviewEditOP.h"

#include <ee/Visitor.h>
#include <ee/Sprite.h>

#include <easyparticle3d.h>
#include <easyanim.h>

namespace ecomplex
{

PreviewEditOP::PreviewEditOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							 const std::vector<const ee::Sprite*>& sprites)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_sprites(sprites)
{
}

class RestartVisitor : public ee::Visitor<ee::Sprite>
{
public:
	virtual void Visit(ee::Sprite* spr, bool& next) {
		if (const eparticle3d::Sprite* particle = dynamic_cast<const eparticle3d::Sprite*>(spr)) {
			const_cast<eparticle3d::Sprite*>(particle)->Start();
		} else if (const eanim::Sprite* anim = dynamic_cast<const eanim::Sprite*>(spr)) {
			const_cast<eanim::Symbol*>(dynamic_cast<const eanim::Symbol*>(anim->GetSymbol()))->ResetTime();
		}
	}
}; // RestartVisitor

bool PreviewEditOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	RestartVisitor restart;
	bool next;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = const_cast<ee::Sprite*>(m_sprites[i]);
		restart.Visit(spr, next);
		dynamic_cast<ee::Symbol*>(spr->GetSymbol())->Traverse(restart);
	}

	return false;
}

}