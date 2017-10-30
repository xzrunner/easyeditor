#include "Scale9Builder.h"
#include "ImageBuilder.h"
#include "IPackNode.h"
#include "PackPicture.h"

#include <easyscale9.h>

#include <ee/Visitor.h>
#include <ee/Exception.h>
#include <ee/ImageSprite.h>

namespace erespacker
{

Scale9Builder::Scale9Builder()
{
}

Scale9Builder::~Scale9Builder()
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

void Scale9Builder::Traverse(ee::Visitor<IPackNode>& visitor) const
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

const IPackNode* Scale9Builder::Create(const std::shared_ptr<const escale9::Sprite>& spr)
{
	if (const IPackNode* node = Query(spr)) {
		return node;
	}

	PackPicture* node = new PackPicture;
	Load(spr, node);

	Value val;
	sm::vec2 sz = spr->GetScale9().GetSize();
	val.w = sz.x;
	val.h = sz.y;
	val.node = node;
	m_map_data.insert(std::make_pair(std::dynamic_pointer_cast<escale9::Symbol>(spr->GetSymbol()), val));
	return node;
}

const IPackNode* Scale9Builder::Query(const std::shared_ptr<const escale9::Sprite>& spr) const
{
	sm::vec2 sz = spr->GetScale9().GetSize();
	auto key = std::dynamic_pointer_cast<escale9::Symbol>(spr->GetSymbol());
	auto itr_s = m_map_data.lower_bound(key);
	auto itr_e = m_map_data.upper_bound(key);
	std::multimap<std::shared_ptr<const escale9::Symbol>, Value>::const_iterator itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		const Value& val = itr->second;
		if (val.w == sz.x && val.h == sz.y) {
			return val.node;
		}
	}

	return NULL;
}

void Scale9Builder::Load(const std::shared_ptr<const escale9::Sprite>& spr, PackPicture* pic)
{
	CU_VEC<s2::SprPtr> grids;
	spr->GetScale9().GetGrids(grids);
	for (int i = 0, n = grids.size(); i < n; ++i)
	{
		auto& grid = grids[i];
		if (auto image = std::dynamic_pointer_cast<ee::ImageSprite>(grid)) {
			PackPicture::Quad quad;
			ImageBuilder::LoadPictureQuad(image, quad, true);
			pic->quads.push_back(quad);
		} else {
			throw ee::Exception("PackPicture::LoadScale9 unknown spr type, filepath: %s", 
				std::dynamic_pointer_cast<ee::Symbol>(image->GetSymbol())->GetFilepath().c_str());
		}
	}
}

}