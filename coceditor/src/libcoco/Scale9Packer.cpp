#include "Scale9Packer.h"
#include "ImagePacker.h"
#include "IPackNode.h"
#include "PackPicture.h"

namespace libcoco
{

Scale9Packer::Scale9Packer()
{
}

Scale9Packer::~Scale9Packer()
{
	std::multimap<const escale9::Symbol*, Value>::iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

void Scale9Packer::PackToLuaString(ebuilder::CodeGenerator& gen,
							 const TexturePacker& tp) const
{
	std::vector<const IPackNode*> nodes;
	nodes.reserve(m_map_data.size());

	std::multimap<const escale9::Symbol*, Value>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
//		itr->second.node->PackToLuaString(gen, tp);
		nodes.push_back(itr->second.node);
	}

	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		nodes[i]->PackToLuaString(gen, tp);
	}
}

const IPackNode* Scale9Packer::Create(const escale9::Sprite* spr)
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

const IPackNode* Scale9Packer::Query(const escale9::Sprite* spr) const
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

void Scale9Packer::Load(const escale9::Sprite* spr, PackPicture* pic)
{
	std::vector<d2d::ISprite*> sprites;
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
		d2d::ISprite* sprite = sprites[i];
		if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite)) {
			PackPicture::Quad quad;
			ImagePacker::LoadPictureQuad(image, quad);
			pic->quads.push_back(quad);
		} else {
			throw d2d::Exception("PackPicture::LoadScale9 unknown spr type.");
		}
	}
}

}