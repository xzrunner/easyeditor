#include "PictureToSpr.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>

#include <easyicon.h>

namespace ecomplex
{

ee::Sprite* PictureToSpr::Trans(const erespacker::PackPicture* pic)
{
	if (pic->quads.size() == 1) {
		return TransQuad(&pic->quads[0]);
	} else if (pic->quads.size() > 1) {
		ecomplex::Symbol* complex = new ecomplex::Symbol;
		for (int i = 0, n = pic->quads.size(); i < n; ++i) {
			complex->Add(TransQuad(&pic->quads[i]));
		}
//		complex->InitBounding();
		return new Sprite(complex);
	} else {
		return NULL;
	}
}

ee::Sprite* PictureToSpr::TransQuad(const erespacker::PackPicture::Quad* quad)
{
	const sm::i16_vec2& sz = quad->img->GetOriginSize();
	
	sm::vec2 src[4], screen[4];
	for (int i = 0; i < 4; ++i) {
		src[i].x = quad->texture_coord[i].x / sz.x;
		src[i].y = 1 - quad->texture_coord[i].y / sz.y;
		screen[i] = quad->screen_coord[i];
	}

	ee::ImageSymbol* img_sym = new ee::ImageSymbol(
		const_cast<ee::Image*>(quad->img), "");
	eicon::QuadIcon* icon = new eicon::QuadIcon(img_sym, src, screen);
	img_sym->RemoveReference();

	eicon::Symbol* sym = new eicon::Symbol;
	sym->SetIcon(icon);
	ee::Sprite* ret = new eicon::Sprite(sym);
	ret->UpdateBounding();
	return ret;
}

}