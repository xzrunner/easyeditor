#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "RenderParams.h"

namespace s2
{

ComplexSprite::ComplexSprite() 
{
}

ComplexSprite::ComplexSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

ComplexSprite* ComplexSprite::Clone() const
{
	return new ComplexSprite(*this);
}

bool ComplexSprite::Update(const RenderParams& params, float dt)
{
	RenderParams p = params;
	p.mt = GetTransMatrix() * params.mt;

	bool dirty = false;
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(p, dt)) {
			dirty = true;
		}
	}
	return dirty;
}

}