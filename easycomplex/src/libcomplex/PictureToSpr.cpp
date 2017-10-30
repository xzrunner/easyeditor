#include "PictureToSpr.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>

#include <easyicon.h>

namespace ecomplex
{

ee::SprPtr PictureToSpr::Trans(const erespacker::PackPicture* pic)
{
	if (pic->quads.size() == 1) {
		return TransQuad(&pic->quads[0]);
	} else if (pic->quads.size() > 1) {
		auto sym = std::make_shared<ecomplex::Symbol>();
		for (int i = 0, n = pic->quads.size(); i < n; ++i) {
			sym->Add(TransQuad(&pic->quads[i]));
		}
//		complex->InitBounding();
		return std::make_shared<Sprite>(sym);
	} else {
		return nullptr;
	}
}

ee::SprPtr PictureToSpr::TransQuad(const erespacker::PackPicture::Quad* quad)
{
	const sm::i16_vec2& sz = quad->img->GetOriginSize();
	
	sm::vec2 src[4], screen[4];
	for (int i = 0; i < 4; ++i) {
		src[i].x = quad->texture_coord[i].x / sz.x;
		src[i].y = 1 - quad->texture_coord[i].y / sz.y;
		screen[i] = quad->screen_coord[i];
	}

	auto img_sym = std::make_shared<ee::ImageSymbol>(quad->img, "");
	std::unique_ptr<s2::Icon> icon = std::make_unique<eicon::QuadIcon>(img_sym, src, screen);

	auto sym = std::make_shared<eicon::Symbol>();
	sym->SetIcon(icon);
	auto ret = std::make_shared<eicon::Sprite>(sym);
	ret->UpdateBounding();
	return ret;
}

}