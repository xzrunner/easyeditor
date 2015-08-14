#include "sprite_utility.h"
#include "ISprite.h"
#include "AbstractBV.h"

namespace d2d
{

float get_spr_left(const ISprite* spr)
{
	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);
	float ret = FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].x < ret) {
			ret = bound[i].x;
		}
	}
	return ret;
}

float get_spr_right(const ISprite* spr)
{
	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);
	float ret = -FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].x > ret) {
			ret = bound[i].x;
		}
	}
	return ret;
}

float get_spr_down(const ISprite* spr)
{
	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);
	float ret = FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].y < ret) {
			ret = bound[i].y;
		}
	}
	return ret;
}

float get_spr_up(const ISprite* spr)
{
	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);
	float ret = -FLT_MAX;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		if (bound[i].y > ret) {
			ret = bound[i].y;
		}
	}
	return ret;
}

float get_spr_center_x(const ISprite* spr)
{
	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);
	float ret = 0;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		ret += bound[i].x;
	}
	return ret / bound.size();
}

float get_spr_center_y(const ISprite* spr)
{
	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);
	float ret = 0;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		ret += bound[i].y;
	}
	return ret / bound.size();

}

}