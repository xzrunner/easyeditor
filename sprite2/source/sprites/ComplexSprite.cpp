#include "ComplexSprite.h"
#include "ComplexSymbol.h"

namespace s2
{

ComplexSprite::ComplexSprite(void* ud)
	: Sprite(ud)
{
}

bool ComplexSprite::Update(float dt)
{
	if (!m_sym) {
		return false;
	}

	bool dirty = false;
	const std::vector<Sprite*>& children = static_cast<ComplexSymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(dt)) {
			dirty = true;
		}
	}
	return dirty;
}

}