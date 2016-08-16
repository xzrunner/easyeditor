#include "IconBuilder.h"
#include "PackAnimation.h"

#include "Scale9Builder.h"
#include "ImageBuilder.h"
#include "IPackNode.h"
#include "PackPicture.h"

#include <ee/Visitor.h>

#include <easyicon.h>

namespace erespacker
{

IconBuilder::IconBuilder()
{
}

IconBuilder::~IconBuilder()
{
	std::multimap<const eicon::Symbol*, Value>::iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

void IconBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	std::multimap<const eicon::Symbol*, Value>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<IPackNode*>(itr->second.node), has_next);
		if (!has_next) {
			break;
		}
	}

}

const IPackNode* IconBuilder::Create(const eicon::Sprite* spr)
{
	if (const IPackNode* node = Query(spr)) {
		return node;
	}

	PackPicture* node = new PackPicture;
	Load(spr, node);

	Value val;
	val.proc = spr->GetProcess();
	val.node = node;
	m_map_data.insert(std::make_pair(dynamic_cast<const eicon::Symbol*>(spr->GetSymbol()), val));
	return node;
}

const IPackNode* IconBuilder::Query(const eicon::Sprite* spr) const
{
	float proc = spr->GetProcess();
	const eicon::Symbol* key = dynamic_cast<const eicon::Symbol*>(spr->GetSymbol());
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

void IconBuilder::Load(const eicon::Sprite* spr, PackPicture* pic)
{
	float proc = spr->GetProcess();

	PackPicture::Quad quad;

	const eicon::Icon* icon = dynamic_cast<const eicon::Symbol*>(spr->GetSymbol())->GetIcon();
	quad.img = icon->GetImage();
	icon->GetTexCoords(proc, quad.texture_coord);
	icon->GetScreenCoords(proc, quad.texture_coord, quad.screen_coord);

	pic->quads.push_back(quad);
}

}