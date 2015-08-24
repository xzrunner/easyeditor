#include "PackPicture.h"

#include <easyscale9.h>

namespace libcoco
{

PackPicture::PackPicture(const d2d::ImageSprite* img)
{
	LoadImage(img);
}

PackPicture::PackPicture(const escale9::Sprite* scale9)
{
	LoadScale9(scale9);
}

void PackPicture::LoadImage(const d2d::ImageSprite* img)
{
	Quad q;

	q.img_symbol = &img->GetSymbol();

	q.texture_coord[0].set(0, 0);
	q.texture_coord[1].set(0, 1);
	q.texture_coord[2].set(1, 1);
	q.texture_coord[3].set(1, 0);

	d2d::Rect r = img->GetSymbol().GetSize();	
	q.screen_coord[0].set(r.xMin, r.yMin);
	q.screen_coord[1].set(r.xMin, r.yMax);
	q.screen_coord[2].set(r.xMax, r.yMax);
	q.screen_coord[3].set(r.xMax, r.yMin);
	TransScreen(q, img);

	m_quads.push_back(q);
}

void PackPicture::LoadScale9(const escale9::Sprite* scale9)
{
	std::vector<d2d::ISprite*> sprites;
	const escale9::Scale9Data& data = scale9->GetScale9Data();
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
			LoadImage(image);
		} else {
			throw d2d::Exception("PackPicture::LoadScale9 unknown spr type.");
		}
	}
}

void PackPicture::TransScreen(Quad& quad, const d2d::ISprite* spr)
{
	d2d::Rect r = quad.img_symbol->GetSize();	
	float hw = r.xLength() * 0.5f,
		hh = r.yLength() * 0.5f;

	// 1. shear
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

	// 2. mirror
	bool xMirror, yMirror;
	spr->GetMirror(xMirror, yMirror);
	if (xMirror) {
		for (size_t i = 0; i < 4; ++i)
			quad.screen_coord[i].x = -quad.screen_coord[i].x;
	}
	if (yMirror) {
		for (size_t i = 0; i < 4; ++i)
			quad.screen_coord[i].y = -quad.screen_coord[i].y;
	}

	// 3. scale
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].x *= spr->GetScale().x;
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].y *= spr->GetScale().y;

	// 4. rotate
	for (size_t i = 0; i < 4; ++i) {
		d2d::Vector rot = d2d::Math::rotateVector(quad.screen_coord[i], spr->GetAngle());
		quad.screen_coord[i] = rot;
	}

	// 5. translate
	d2d::Vector center = spr->GetCenter();
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i] += center;

	// flip y
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].y = -quad.screen_coord[i].y;

	// final scale
	const float SCALE = 16;
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i] *= SCALE;
}

}