#include "MaskSprite.h"
#include "MaskSymbol.h"

namespace s2
{

MaskSprite::MaskSprite() 
{
}

MaskSprite::MaskSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

MaskSprite* MaskSprite::Clone() const
{
	return new MaskSprite(*this);
}

bool MaskSprite::Update(const RenderParams& params)
{
	bool dirty = false;
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		if (const_cast<Sprite*>(base)->Update(params)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		if (const_cast<Sprite*>(mask)->Update(params)) {
			dirty = true;
		}
	}
	return dirty;
}

}