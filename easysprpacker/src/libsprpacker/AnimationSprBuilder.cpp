#include "AnimationSprBuilder.h"
#include "PackAnimationSpr.h"

#include <easyanim.h>

namespace esprpacker
{

SINGLETON_DEFINITION(AnimationSprBuilder);

AnimationSprBuilder::AnimationSprBuilder()
{
}

AnimationSprBuilder::~AnimationSprBuilder()
{
	Clear();
}

void AnimationSprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const eanim::Symbol*, const PackAnimationSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimationSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void AnimationSprBuilder::Clear()
{
	std::multimap<const eanim::Symbol*, const PackAnimationSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* AnimationSprBuilder::Create(const eanim::Sprite* spr)
{
	bool loop = spr->IsLoop();
	float interval = spr->GetInterval();
	int fps = spr->GetFPS();

	const eanim::Symbol* key = dynamic_cast<const eanim::Symbol*>(spr->GetSymbol());
	std::multimap<const eanim::Symbol*, const PackAnimationSpr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->GetLoop() == loop &&
			itr->second->GetInterval() == interval &&
			itr->second->GetFPS() == fps) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	PackAnimationSpr* node = new PackAnimationSpr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}