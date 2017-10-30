#include "render_utility.h"
#include "Image.h"
#include "ImageSprite.h"
#include "ImageSymbol.h"

#include <sprite2/BoundingBox.h>
#include <sprite2/RenderTarget.h>
#include <sprite2/DrawRT.h>

namespace ee
{

SprPtr draw_all_to_one_spr(const std::vector<SprPtr>& sprs, const SprPtr& except)
{
	std::vector<SprPtr> _sprites;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		if (sprs[i] != except) {
			_sprites.push_back(sprs[i]);
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

SprPtr draw_all_to_one_spr(const std::vector<SprPtr>& sprs)
{
	if (sprs.empty()) {
		return NULL;
	}

	sm::rect r;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->GetBounding()->CombineTo(r);
	}

	sm::vec2 sz = r.Size();
	float scale = cal_texture_size(sz.x, sz.y);
	sz *= scale;
	sm::vec2 center = r.Center() * scale;

	auto rt = std::make_shared<s2::RenderTarget>(static_cast<int>(sz.x), static_cast<int>(sz.y));

	s2::DrawRT draw(rt.get());
	for (auto& spr : sprs) {
		draw.Draw(spr.get(), false, static_cast<int>(sz.x + 0.5f), static_cast<int>(sz.y + 0.5f), center.x, center.y, scale);
	}

	auto img = std::make_shared<Image>(rt);
	auto sym = std::make_shared<ImageSymbol>(img, "ss");
	auto spr = std::make_shared<ImageSprite>(sym);

	spr->SetPosition(r.Center());
	spr->SetAngle(0);
	spr->SetScale(sm::vec2(1.0f / scale, - 1.0f / scale));

	return spr;
}

}