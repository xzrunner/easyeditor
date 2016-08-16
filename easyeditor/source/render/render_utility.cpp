#include "render_utility.h"
#include "Snapshoot.h"
#include "Image.h"
#include "ImageSprite.h"
#include "ImageSymbol.h"

#include <sprite2/BoundingBox.h>

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

	sm::rect r;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->GetBounding()->CombineTo(r);
	}

	sm::vec2 sz = r.Size();
	float scale = cal_texture_size(sz.x, sz.y);
	sz *= scale;
	sm::vec2 center = r.Center() * scale;

	Snapshoot ss(static_cast<int>(sz.x), static_cast<int>(sz.y));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		ss.DrawSprite(spr, false, static_cast<int>(sz.x + 0.5f), static_cast<int>(sz.y + 0.5f), center.x, center.y, scale);
	}

	Image* img = new Image(ss.GetFBO());
	ImageSymbol* sym = new ImageSymbol(img, "ss");
	img->RemoveReference();
	ImageSprite* spr = new ImageSprite(sym);
	sym->RemoveReference();

	spr->SetMirror(sm::bvec2(false, true));
	spr->SetPosition(r.Center());
	spr->SetAngle(0);
	spr->SetScale(sm::vec2(1.0f / scale, 1.0f / scale));

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