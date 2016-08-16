#include "MaxRectsBinaryArrange.h"

#include <ee/ImageSprite.h>

#include <easytexpacker.h>

namespace etexpacker
{

void MaxRectsBinaryArrange::Arrange(const std::vector<ee::ImageSprite*>& sprites)
{
	int sz = sprites.size();
	std::vector<etexpacker::RectSize> rects;
	rects.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		sm::rect r = sprites[i]->GetSymbol()->GetBounding();
		rects.push_back(etexpacker::RectSize(r.Width(), r.Height()));
	}
	std::vector<etexpacker::Rect> output;

	etexpacker::MaxRectsBinaryPack2 pack;
	pack.Pack(etexpacker::PACK_AUTO, 2048, 2048, 2048, rects, output);

	assert(sprites.size() == output.size());
	for (int i = 0; i < sz; ++i) {
		ee::Sprite* sprite = sprites[i];
		const etexpacker::Rect& r = output[i];
		sm::vec2 pos;
		pos.x = r.x + r.width * 0.5f;
		pos.y = r.y + r.height * 0.5f;
		sprite->SetPosition(pos);
	}
}

}