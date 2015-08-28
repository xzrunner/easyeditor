#include "AnimBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easyanim.h>

namespace libcoco
{

AnimBuilder::AnimBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

AnimBuilder::~AnimBuilder()
{
	std::map<const libanim::Symbol*, const PackAnimation*>::iterator
		itr = m_map_anim.begin();
	for ( ; itr != m_map_anim.end(); ++itr) {
		delete itr->second;
	}
}

const IPackNode* AnimBuilder::Create(const libanim::Symbol* symbol)
{
	std::map<const libanim::Symbol*, const PackAnimation*>::iterator 
		itr = m_map_anim.find(symbol);
	if (itr != m_map_anim.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(symbol, node);
	return node;
}

void AnimBuilder::Load(const libanim::Symbol* symbol, PackAnimation* anim)
{
	m_export_set.LoadExport(symbol, anim);

	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = symbol->getMaxFrameIndex(); i <= n; ++i)
	{
		PackAnimation::Frame frame;

		std::vector<d2d::ISprite*> sprites;
		libanim::Utility::GetCurrSprites(symbol, i, sprites);
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			anim->CreateFramePart(sprites[i], frame);
		}

		anim->frames.push_back(frame);
	}

	PackAnimation::Action action;
	action.size = anim->frames.size();
	anim->actions.push_back(action);
}

}