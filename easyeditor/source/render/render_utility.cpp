#include "render_utility.h"
#include "Snapshoot.h"
#include "BoundingBox.h"
#include "Image.h"
#include "ImageSprite.h"

#include <gl/glew.h>

namespace ee
{

Sprite* draw_all_to_one_spr(const std::vector<Sprite*>& sprites, Sprite* except)
{
	std::vector<Sprite*> _sprites;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		if (sprites[i] != except) {
			_sprites.push_back(sprites[i]);
		}
	}
	return draw_all_to_one_spr(sprites);
}

Sprite* draw_all_to_one_spr(const std::vector<Sprite*>& sprites)
{
	if (sprites.empty()) {
		return NULL;
	}

	Rect r;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		std::vector<Vector> bound;
		sprites[i]->GetBounding()->GetBoundPos(bound);
		for (int j = 0, m = bound.size(); j < m; ++j) {
			r.Combine(bound[j]);
		}
	}

	int dx = static_cast<int>(r.CenterX()),
		dy = static_cast<int>(r.CenterY());
	Snapshoot ss(r.Width(), r.Height());
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
	GLenum err;
	while ( ( err = glGetError() ) != GL_NO_ERROR) {
		int zz = err;
		std::cerr << err;        
	}
}

}