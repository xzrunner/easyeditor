#include "ComplexBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <drag2d.h>
#include <easycomplex.h>

namespace libcoco
{

ComplexBuilder::ComplexBuilder()
{
}

ComplexBuilder::~ComplexBuilder()
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator
		itr = m_map_complex.begin();
	for ( ; itr != m_map_complex.end(); ++itr) {
		delete itr->second;
	}
}

const IPackNode* ComplexBuilder::Create(const ecomplex::Symbol* symbol)
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator 
		itr = m_map_complex.find(symbol);
	if (itr != m_map_complex.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(symbol, node);
	return node;
}

void ComplexBuilder::Load(const ecomplex::Symbol* symbol, PackAnimation* anim)
{
  	// todo: only one action
	PackAnimation::Action action;
  	action.size = symbol->m_sprites.size();
	anim->m_actions.push_back(action);

	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = symbol->m_sprites.size(); i < n; ++i)
	{
		const IPackNode* node = factory->Create(symbol->m_sprites[i]);

		PackAnimation::Frame frame;
		frame.comp_idx = anim->AddComponent(node);

		anim->m_frames.push_back(frame);
	}
}

}