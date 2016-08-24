#include "Symbol.h"
#include "FileIO.h"

#include <easyparticle3d.h>
#include <easycomplex.h>

namespace emask
{

void Symbol::Update(const s2::RenderParams& params, float dt)
{
	UpdateP3DSymbol(const_cast<s2::Symbol*>(m_base), params, dt);
	UpdateP3DSymbol(const_cast<s2::Symbol*>(m_mask), params, dt);
}

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

void Symbol::UpdateP3DSymbol(s2::Symbol* sym, const s2::RenderParams& params, float dt)
{
	if (eparticle3d::Symbol* p3d = dynamic_cast<eparticle3d::Symbol*>(sym)) {
		p3d->Update(params, dt);
	} else if (ecomplex::Symbol* comp = dynamic_cast<ecomplex::Symbol*>(sym)) {
		const std::vector<s2::Sprite*>& children = comp->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (eparticle3d::Sprite* p3d = dynamic_cast<eparticle3d::Sprite*>(children[i])) {
				p3d->Update(params, dt);
			}
		}
	}
}

}