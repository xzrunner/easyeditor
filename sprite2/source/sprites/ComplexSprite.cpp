#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "RenderParams.h"
#include "ProxySprite.h"
#include "ProxySymbol.h"

namespace s2
{

ComplexSprite::ComplexSprite() 
	: m_action(-1)
{
}

ComplexSprite::ComplexSprite(Symbol* sym) 
	: Sprite(sym) 
	, m_action(-1)
{
}

ComplexSprite* ComplexSprite::Clone() const
{
	return new ComplexSprite(*this);
}

bool ComplexSprite::Update(const RenderParams& params)
{
	RenderParams p = params;
	p.mt = GetTransMatrix() * params.mt;

	bool dirty = false;
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(p)) {
			dirty = true;
		}
	}
	return dirty;
}

Sprite* ComplexSprite::FetchChild(const std::string& name) const
{
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		Sprite* child = children[i];
		if (child->GetName() == name) {
			return child;
		}
		// proxy
		if (ProxySprite::IsProxyName(child->GetName())) 
		{
			ProxySprite* proxy_spr = dynamic_cast<ProxySprite*>(child);
			if (proxy_spr) 
			{
				Sprite* real_spr = VI_DOWNCASTING<ProxySymbol*>(child->GetSymbol())->GetReal();
				if (real_spr->GetName() == name) {
					return real_spr;
				}
			}
		}
	}
	return NULL;
}

void ComplexSprite::SetAction(const std::string& name)
{
	m_action = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionIdx(name);
}

}