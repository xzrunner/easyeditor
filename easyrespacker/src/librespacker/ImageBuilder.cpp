#include "ImageBuilder.h"
#include "IPackNode.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>
#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/Image.h>

#include <sprite2/S2_Symbol.h>
#include <SM_Calc.h>

#include <algorithm>

namespace erespacker
{

ImageBuilder::ImageBuilder()
{
}

ImageBuilder::~ImageBuilder()
{
	std::map<const s2::Symbol*, std::vector<PackPicture*> >::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) {
		for_each(itr->second.begin(), itr->second.end(), ee::DeletePointerFunctor<IPackNode>());	
	}
}

void ImageBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	std::map<const s2::Symbol*, std::vector<PackPicture*> >::const_iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) 
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			bool has_next;
			visitor.Visit(itr->second[i], has_next);
			if (!has_next) {
				return;
			}			
		}
	}
}

const IPackNode* ImageBuilder::Create(const ee::ImageSprite* spr)
{
	PackPicture::Quad quad;
	LoadPictureQuad(spr, quad);

	// query
	std::map<const s2::Symbol*, std::vector<PackPicture*> >::iterator itr 
		= m_nodes.find(spr->GetSymbol());
	if (itr != m_nodes.end()) {
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			PackPicture::Quad* old = &itr->second[i]->quads[0];
			if (old->img == quad.img &&
				memcmp(old->texture_coord, quad.texture_coord, sizeof(old->texture_coord)) == 0 &&
				memcmp(old->screen_coord, quad.screen_coord, sizeof(old->screen_coord)) == 0) {
				return itr->second[i];
			}
		}
	}

	// create
	PackPicture* node = new PackPicture;
	node->quads.push_back(quad);
	if (itr == m_nodes.end()) {
		std::vector<PackPicture*> nodes;
		nodes.push_back(node);
		m_nodes.insert(std::make_pair(spr->GetSymbol(), nodes));
	} else {
		itr->second.push_back(node);
	}
	return node;
}

void ImageBuilder::LoadPictureQuad(const ee::ImageSprite* img, PackPicture::Quad& quad)
{
	quad.img = dynamic_cast<const ee::ImageSymbol*>(img->GetSymbol())->GetImage();

	quad.texture_coord[0].Set(0, 0);
	quad.texture_coord[1].Set(0, 1);
	quad.texture_coord[2].Set(1, 1);
	quad.texture_coord[3].Set(1, 0);

	sm::rect r = img->GetSymbol()->GetBounding();
 	quad.screen_coord[0].Set(r.xmin, r.ymin);
 	quad.screen_coord[1].Set(r.xmin, r.ymax);
 	quad.screen_coord[2].Set(r.xmax, r.ymax);
 	quad.screen_coord[3].Set(r.xmax, r.ymin);	

	TransScreen(quad, img);
}

void ImageBuilder::TransScreen(PackPicture::Quad& quad, const ee::Sprite* spr)
{
	// 1. shear
	float hw = quad.img->GetClippedWidth() * 0.5f,
		hh = quad.img->GetClippedHeight() * 0.5f;
	float sx = spr->GetShear().x,
		sy = spr->GetShear().y;
	quad.screen_coord[1].x += sx * hh;
	quad.screen_coord[2].x += sx * hh;
	quad.screen_coord[0].x -= sx * hh;
	quad.screen_coord[3].x -= sx * hh;
	quad.screen_coord[2].y += sy * hw;
	quad.screen_coord[3].y += sy * hw;
	quad.screen_coord[1].y -= sy * hw;
	quad.screen_coord[0].y -= sy * hw;

	// 3. scale
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].x *= spr->GetScale().x;
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].y *= spr->GetScale().y;

	// 4. rotate
	for (size_t i = 0; i < 4; ++i) {
		sm::vec2 rot = sm::rotate_vector(quad.screen_coord[i], spr->GetAngle());
		quad.screen_coord[i] = rot;
	}

	// 5. translate
	sm::vec2 center = spr->GetCenter();
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i] += center;

// 	// flip y
// 	for (size_t i = 0; i < 4; ++i)
// 		quad.screen_coord[i].y = -quad.screen_coord[i].y;
// 
// 	// final scale
// 	const float SCALE = 16;
// 	for (size_t i = 0; i < 4; ++i)
// 		quad.screen_coord[i] *= SCALE;
}

}