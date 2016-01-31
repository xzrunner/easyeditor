#include "AnimBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easyanim.h>

namespace librespacker
{

AnimBuilder::AnimBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

AnimBuilder::~AnimBuilder()
{
	std::map<const libanim::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void AnimBuilder::Traverse(ee::Visitor& visitor) const
{
 	std::map<const libanim::Symbol*, const PackAnimation*>::const_iterator 
 		itr = m_map_data.begin();
 	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
 	}
}

const IPackNode* AnimBuilder::Create(const libanim::Symbol* symbol)
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

void AnimBuilder::Load(const libanim::Symbol* symbol, PackAnimation* anim)
{
	m_export_set.LoadExport(symbol, anim);

	for (int i = 1, n = symbol->getMaxFrameIndex(); i <= n; ++i)
	{
		PackAnimation::Frame frame;

		std::vector<ee::Sprite*> sprites;
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