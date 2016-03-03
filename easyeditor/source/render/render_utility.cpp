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
	return draw_all_to_one_spr(_sprites);
}

float cal_texture_size(float w, float h)
{
	const float MAX_SIZE = 4096;

	float max = std::max(w, h);
	if (max <= MAX_SIZE) {
		return 1;
	} else {
		return MAX_SIZE / max;
	}
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

	float scale = cal_texture_size(r.Width(), r.Height());
	float w = r.Width() * scale,
		h = r.Height() * scale;
	float dx = r.CenterX() * scale,
		dy = r.CenterY() * scale;

	Snapshoot ss(static_cast<int>(w), static_cast<int>(h));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		ss.DrawSprite(spr, false, static_cast<int>(w), static_cast<int>(h), dx, dy, scale);
	}

	Image* img = new Image(ss.GetFBO());
	ImageSymbol* symbol = new ImageSymbol(img, "ss");
	ImageSprite* spr = new ImageSprite(symbol);

	spr->SetMirror(false, true);
	spr->SetTransform(Vector(dx, dy), 0);
	spr->SetScale(ee::Vector(1.0f / scale, 1.0f / scale));

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