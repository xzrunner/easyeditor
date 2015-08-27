#include "IconBuilder.h"

namespace libcoco
{

IconBuilder::IconBuilder()
{
}

IconBuilder::~IconBuilder()
{
	std::map<const eicon::Symbol*, Value>::iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

const IPackNode* IconBuilder::Create(const eicon::Sprite* spr)
{
	if (const IPackNode* node = Query(spr)) {
		return node;
	}

	PackAnimation* node = new PackAnimation;
	Load(spr, node);

	Value val;
	val.proc = spr->GetProcess();
	val.node = node;
	m_map_data.insert(std::make_pair(&spr->GetSymbol(), val));
	return node;
}

const IPackNode* IconBuilder::Query(const eicon::Sprite* spr) const
{
	float proc = spr->GetProcess();
	const eicon::Symbol* key = &spr->GetSymbol();
	std::multimap<const eicon::Symbol*, Value>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		const Value& val = itr->second;
		if (val.proc == proc) {
			return val.node;
		}
	}

	return NULL;
}

void IconBuilder::Load(const eicon::Sprite* spr, PackAnimation* anim)
{
	
}

}