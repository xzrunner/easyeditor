#include "TweenUtility.h"

#include <ee/RenderParams.h>

#include <easyscale9.h>
#include <easyicon.h>
#include <easymesh.h>

namespace eanim
{

void TweenUtility::GetTweenSprites(const std::vector<ee::Sprite*>& start, const std::vector<ee::Sprite*>& end, 
	std::vector<ee::Sprite*>& tween, float process)
{
	for (size_t i = 0, n = start.size(); i < n; ++i)
	{
		ee::Sprite *start_spr = start[i], *end_spr = NULL;
		for (size_t j = 0, m = end.size(); j < m; ++j)
		{
			if (IsTweenMatched(start_spr, end[j]))
			{
				end_spr = end[j];
				break;
			}
		}

		if (end_spr)
		{
			ee::Sprite* tween_spr = start_spr->Clone();
			GetTweenSprite(start_spr, end_spr, tween_spr, process);
			tween.push_back(tween_spr);
		}
		else
		{
			tween.push_back(start_spr->Clone());
		}
	}
}

bool TweenUtility::IsTweenMatched(const ee::Sprite* s0, const ee::Sprite* s1)
{
	bool auto_named = false;
	if (!s0->name.empty() && s0->name[0] == '_' && !s1->name.empty() && s1->name[0] == '_') {
		auto_named = true;
	}

	if (auto_named && s0->name == s1->name) {
		return true;
	} else if (s0->name.empty() && s1->name.empty()) {
		//		return s0->getSymbol().getFilepath() == s1->getSymbol().getFilepath();
		return false;
	} else {
		return s0->name == s1->name;
	}
}

static inline
s2::Color color_interpolate(const s2::Color& begin, const s2::Color& end, float scale)
{
	s2::Color ret;
	ret.r = begin.r + (end.r - begin.r) * scale;
	ret.g = begin.g + (end.g - begin.g) * scale;
	ret.b = begin.b + (end.b - begin.b) * scale;
	ret.a = begin.a + (end.a - begin.a) * scale;
	return ret;
}

void TweenUtility::GetTweenSprite(ee::Sprite* start, ee::Sprite* end, ee::Sprite* tween, float process)
{
 	sm::vec2 shear;
 	shear.x = (end->GetShear().x - start->GetShear().x) * process + start->GetShear().x;
 	shear.y = (end->GetShear().y - start->GetShear().y) * process + start->GetShear().y;
 	tween->SetShear(shear);

	float xscale = (end->GetScale().x - start->GetScale().x) * process + start->GetScale().x,
		yscale = (end->GetScale().y - start->GetScale().y) * process + start->GetScale().y;
	tween->SetScale(sm::vec2(xscale, yscale));

	sm::vec2 offset = (end->GetOffset() - start->GetOffset()) * process + start->GetOffset();
	tween->SetOffset(offset);

	tween->SetTransform(sm::vec2(0, 0), 0);

	sm::vec2 center_s = start->GetCenter(), center_e = end->GetCenter();

	float angle = (end->GetAngle() - start->GetAngle()) * process + start->GetAngle();
	sm::vec2 base_s = start->GetPosition() + start->GetOffset(),
		base_e = end->GetPosition() + end->GetOffset();
	sm::vec2 base_t = (base_e - base_s) * process + base_s;

	sm::vec2 pos_t = base_t -  offset;
	tween->SetTransform(pos_t, angle);

	tween->GetColor().add = color_interpolate(start->GetColor().add, end->GetColor().add, process);
	tween->GetColor().mul = color_interpolate(start->GetColor().mul, end->GetColor().mul, process);

	if (escale9::Sprite* s9_s = dynamic_cast<escale9::Sprite*>(start))
	{
		escale9::Sprite* s9_e = dynamic_cast<escale9::Sprite*>(end);
		escale9::Sprite* s9_t = dynamic_cast<escale9::Sprite*>(tween);
		assert(s9_e && s9_t);
		float s_w, s_h, e_w, e_h;
		s9_s->GetSize(s_w, s_h);
		s9_e->GetSize(e_w, e_h);
		float t_w = (e_w - s_w) * process + s_w,
			t_h = (e_h - s_h) * process + s_h;
		s9_t->SetSize(t_w, t_h);
	}
	else if (eicon::Sprite* icon_s = dynamic_cast<eicon::Sprite*>(start))
	{
		eicon::Sprite* icon_e = dynamic_cast<eicon::Sprite*>(end);
		eicon::Sprite* icon_t = dynamic_cast<eicon::Sprite*>(tween);
		assert(icon_e && icon_t);
		float proc = (icon_e->GetProcess() - icon_s->GetProcess()) * process + icon_s->GetProcess();
		icon_t->SetProcess(proc);
	}
	else if (emesh::Sprite* mesh_s = dynamic_cast<emesh::Sprite*>(start))
	{
		emesh::Sprite* mesh_e = dynamic_cast<emesh::Sprite*>(end);
		emesh::Sprite* mesh_t = dynamic_cast<emesh::Sprite*>(tween);
		assert(mesh_e && mesh_t);
		mesh_t->SetTween(mesh_s, mesh_e, process);
	}
}

}