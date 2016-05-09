#include "PictureToSpr.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/Image.h>

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
			complex->m_sprites.push_back(TransQuad(&pic->quads[i]));
		}
		complex->InitBounding();
		return new Sprite(complex);
	} else {
		return NULL;
	}
}

ee::Sprite* PictureToSpr::TransQuad(const erespacker::PackPicture::Quad* quad)
{
	float w = quad->img->GetOriginWidth(),
		h = quad->img->GetOriginHeight();

	sm::vec2 src[4], screen[4];
	for (int i = 0; i < 4; ++i) {
		src[i].x = quad->texture_coord[i].x / w;
		src[i].y = 1 - quad->texture_coord[i].y / h;
		screen[i] = quad->screen_coord[i];
	}

	eicon::QuadIcon* icon = new eicon::QuadIcon(
		const_cast<ee::Image*>(quad->img), src, screen);

	eicon::Symbol* symbol = new eicon::Symbol;
	symbol->SetIcon(icon);
	ee::Sprite* ret = new eicon::Sprite(symbol);
	ret->BuildBounding();
	return ret;
}

}