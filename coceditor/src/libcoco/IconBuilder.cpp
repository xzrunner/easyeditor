#include "IconBuilder.h"
#include "PackAnimation.h"

#include "Scale9Builder.h"
#include "ImageBuilder.h"
#include "IPackNode.h"
#include "PackPicture.h"

namespace libcoco
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

void IconBuilder::ToString(ebuilder::CodeGenerator& gen,
						   const TexturePacker& tp) const
{
	std::multimap<const eicon::Symbol*, Value>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->second.node->ToString(gen, tp);
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

void IconBuilder::Load(const eicon::Sprite* spr, PackPicture* pic)
{
	float proc = spr->GetProcess();

	PackPicture::Quad quad;

	const eicon::Icon* icon = spr->GetSymbol().GetIcon();
	
	icon->GetTexcoords4(quad.texture_coord, proc);

	const d2d::Image* img = icon->GetImage();
	float w = img->GetClippedWidth(), h = img->GetClippedHeight();
	float hw = w * 0.5f, hh = h * 0.5f;
	float dx = img->GetOffset().x, dy = img->GetOffset().y;
	float xmin = -hw + dx, ymin = -hh + dy;
	for (int i = 0; i < 4; ++i) {
		quad.screen_coord[i].x = -hw + w * quad.texture_coord[i].x;
		quad.screen_coord[i].y = -hh + h * quad.texture_coord[i].y;
	}

	pic->quads.push_back(quad);
}

}