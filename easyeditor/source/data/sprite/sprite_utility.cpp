#include "sprite_utility.h"
#include "Sprite.h"

#include <float.h>

namespace ee
{

float get_spr_left(const Sprite* spr)
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	float ret = FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].x < ret) {
			ret = bound[i].x;
		}
	}
	return ret;
}

float get_spr_right(const Sprite* spr)
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	float ret = -FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].x > ret) {
			ret = bound[i].x;
		}
	}
	return ret;
}

float get_spr_down(const Sprite* spr)
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	float ret = FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].y < ret) {
			ret = bound[i].y;
		}
	}
	return ret;
}

float get_spr_up(const Sprite* spr)
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	float ret = -FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].y > ret) {
			ret = bound[i].y;
		}
	}
	return ret;
}

float get_spr_center_x(const Sprite* spr)
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	float ret = 0;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		ret += bound[i].x;
	}
	return ret / bound.size();
}

float get_spr_center_y(const Sprite* spr)
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	float ret = 0;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		ret += bound[i].y;
	}
	return ret / bound.size();

}

}