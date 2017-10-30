#include "IconBuilder.h"
#include "PackAnimation.h"

#include "Scale9Builder.h"
#include "ImageBuilder.h"
#include "IPackNode.h"
#include "PackPicture.h"

#include <ee/Visitor.h>
#include <ee/ImageSymbol.h>

#include <easyicon.h>

namespace erespacker
{

IconBuilder::IconBuilder()
{
}

IconBuilder::~IconBuilder()
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

void IconBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<IPackNode*>(itr->second.node), has_next);
		if (!has_next) {
			break;
		}
	}

}

const IPackNode* IconBuilder::Create(const std::shared_ptr<const eicon::Sprite>& spr)
{
	if (const IPackNode* node = Query(spr.get())) {
		return node;
	}

	PackPicture* node = new PackPicture;
	Load(spr.get(), node);

	Value val;
	val.proc = spr->GetProcess();
	val.node = node;
	m_map_data.insert(std::make_pair(std::dynamic_pointer_cast<eicon::Symbol>(spr->GetSymbol()), val));
	return node;
}

const IPackNode* IconBuilder::Query(const eicon::Sprite* spr) const
{
	float proc = spr->GetProcess();
	auto key = std::dynamic_pointer_cast<eicon::Symbol>(spr->GetSymbol());
	auto itr_s = m_map_data.lower_bound(key);
	auto itr_e = m_map_data.upper_bound(key);
	std::multimap<std::shared_ptr<const eicon::Symbol>, Value>::const_iterator itr;
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

	auto sym = std::dynamic_pointer_cast<eicon::Symbol>(spr->GetSymbol());
	auto icon = dynamic_cast<const eicon::Icon*>(sym->GetIcon().get());
	quad.img = std::dynamic_pointer_cast<ee::ImageSymbol>(icon->GetImage())->GetImage();
	icon->GenTexcoords(proc, quad.texture_coord);
	icon->GenVertices(proc, quad.texture_coord, quad.screen_coord);

	pic->quads.push_back(quad);
}

}