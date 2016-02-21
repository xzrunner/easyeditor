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
	std::multimap<const escale9::Symbol*, Value>::iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

void Scale9Builder::Traverse(ee::Visitor& visitor) const
{
	std::multimap<const escale9::Symbol*, Value>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<IPackNode*>(itr->second.node), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* Scale9Builder::Create(const escale9::Sprite* spr)
{
	if (const IPackNode* node = Query(spr)) {
		return node;
	}

	PackPicture* node = new PackPicture;
	Load(spr, node);

	Value val;
	float w, h;
	spr->GetSize(w, h);
	val.w = w;
	val.h = h;
	val.node = node;
	m_map_data.insert(std::make_pair(&spr->GetSymbol(), val));
	return node;
}

const IPackNode* Scale9Builder::Query(const escale9::Sprite* spr) const
{
	float w, h;
	spr->GetSize(w, h);

	const escale9::Symbol* key = &spr->GetSymbol();
	std::multimap<const escale9::Symbol*, Value>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		const Value& val = itr->second;
		if (val.w == w && val.h == h) {
			return val.node;
		}
	}

	return NULL;
}

void Scale9Builder::Load(const escale9::Sprite* spr, PackPicture* pic)
{
	std::vector<ee::Sprite*> sprites;
	const escale9::Scale9Data& data = spr->GetScale9Data();
	switch (data.GetType())
	{
	case escale9::e_9Grid:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
				sprites.push_back(data.GetSprite(i, j));
		break;
	case escale9::e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				sprites.push_back(data.GetSprite(i, j));
			}
		}
		break;
	case escale9::e_3GridHor:
		for (size_t i = 0; i < 3; ++i)
			sprites.push_back(data.GetSprite(1, i));
		break;
	case escale9::e_3GridVer:
		for (size_t i = 0; i < 3; ++i)
			sprites.push_back(data.GetSprite(i, 1));
		break;
	case escale9::e_6GridUpper:
		for (size_t i = 1; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
				sprites.push_back(data.GetSprite(i, j));
		break;
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];
		if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(sprite)) {
			PackPicture::Quad quad;
			ImageBuilder::LoadPictureQuad(image, quad);
			pic->quads.push_back(quad);
		} else {
			throw ee::Exception("PackPicture::LoadScale9 unknown spr type, filepath: %s", image->GetSymbol().GetFilepath().c_str());
		}
	}
}

}