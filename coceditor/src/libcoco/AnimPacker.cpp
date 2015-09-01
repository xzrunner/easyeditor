#include "AnimPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easyanim.h>

namespace libcoco
{

AnimPacker::AnimPacker(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

AnimPacker::~AnimPacker()
{
	std::map<const libanim::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void AnimPacker::PackToLuaString(ebuilder::CodeGenerator& gen,
						   const TexturePacker& tp) const
{
	std::map<const libanim::Symbol*, const PackAnimation*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->second->PackToLuaString(gen, tp);
	}
}

const IPackNode* AnimPacker::Create(const libanim::Symbol* symbol)
{
	std::map<const libanim::Symbol*, const PackAnimation*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void AnimPacker::Load(const libanim::Symbol* symbol, PackAnimation* anim)
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