#include "render_utility.h"
#include "Snapshoot.h"

#include "dataset/AbstractBV.h"
#include "dataset/Image.h"
#include "dataset/ImageSprite.h"
#include "view/MultiSpritesImpl.h"

#include <gl/glew.h>

namespace d2d
{

ISprite* draw_all_to_one_spr(const MultiSpritesImpl* sprites_impl, ISprite* except)
{
	std::vector<ISprite*> sprites;
	sprites_impl->TraverseSprites(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		if (sprites[i] == except) {
			sprites.erase(sprites.begin() + i);
			break;
		}
	}

	return draw_all_to_one_spr(sprites);
}

ISprite* draw_all_to_one_spr(const std::vector<ISprite*>& sprites)
{
	if (sprites.empty()) {
		return NULL;
	}

	Rect r;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		std::vector<Vector> bound;
		sprites[i]->GetBounding()->getBoundPos(bound);
		for (int j = 0; j < bound.size(); ++j) {
			r.combine(bound[j]);
		}
	}

	float dx = r.xCenter(),
		dy = r.yCenter();
	Snapshoot ss(r.xLength(), r.yLength());
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ss.DrawSprite(sprites[i], false, dx, dy);
	}

	Image* img = new Image(ss.GetFBO());
	ImageSymbol* symbol = new ImageSymbol(img, "ss");
	ImageSprite* spr = new ImageSprite(symbol);

	spr->SetMirror(false, true);
	spr->SetTransform(Vector(dx, dy), 0);

	return spr;
}

void gl_debug()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR
		//		&& error != GL_INVALID_ENUM 
		//		&& error != GL_INVALID_VALUE
		//		&& error != GL_INVALID_OPERATION
		//		&& error != GL_OUT_OF_MEMORY
		//		&& error != GL_STACK_OVERFLOW 
		//		&& error != GL_STACK_UNDERFLOW
		) {
			int zz = 0;
			exit(1);
	}
}

}