#include "MaxRectsBinaryArrange.h"

#include <easypacker.h>

namespace epacker
{

void MaxRectsBinaryArrange::arrange(const std::vector<d2d::ImageSprite*>& sprites)
{
	int sz = sprites.size();
	std::vector<libpacker::RectSize> rects;
	rects.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		d2d::Rect r = sprites[i]->GetSymbol().GetSize();
		rects.push_back(libpacker::RectSize(r.xLength(), r.yLength()));
	}
	std::vector<libpacker::Rect> output;

	libpacker::MaxRectsBinaryPack2 pack;
	pack.Pack(libpacker::PACK_AUTO, rects, output);

	assert(sprites.size() == output.size());
	for (int i = 0; i < sz; ++i) {
		d2d::ISprite* sprite = sprites[i];
		const libpacker::Rect& r = output[i];
		d2d::Vector pos;
		pos.x = r.x + r.width * 0.5f;
		pos.y = r.y + r.height * 0.5f;
		sprite->SetTransform(pos, sprite->GetAngle());
	}
}

}