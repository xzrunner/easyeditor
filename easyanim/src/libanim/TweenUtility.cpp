#include "TweenUtility.h"

#include <easyicon.h>

namespace libanim
{

void TweenUtility::GetTweenSprites(const std::vector<d2d::ISprite*>& start, const std::vector<d2d::ISprite*>& end, 
	std::vector<d2d::ISprite*>& tween, float process)
{
	for (size_t i = 0, n = start.size(); i < n; ++i)
	{
		d2d::ISprite *start_spr = start[i], *end_spr = NULL;
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
			d2d::ISprite* tween_spr = start_spr->Clone();
			GetTweenSprite(start_spr, end_spr, tween_spr, process);
			tween.push_back(tween_spr);
		}
		else
		{
			tween.push_back(start_spr->Clone());
		}
	}
}

bool TweenUtility::IsTweenMatched(const d2d::ISprite* s0, const d2d::ISprite* s1)
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

void TweenUtility::GetTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process)
{
	float xscale = (end->getScale().x - start->getScale().x) * process + start->getScale().x,
		yscale = (end->getScale().y - start->getScale().y) * process + start->getScale().y;
	tween->setScale(xscale, yscale);

	d2d::Vector offset = (end->getOffset() - start->getOffset()) * process + start->getOffset();
	tween->setOffset(offset);

	tween->setTransform(d2d::Vector(0, 0), 0);

	d2d::Vector center_s = start->getCenter(),
		center_e = end->getCenter();

	float angle = (end->getAngle() - start->getAngle()) * process + start->getAngle();
	d2d::Vector base_s = start->getPosition() + start->getOffset(),
		base_e = end->getPosition() + end->getOffset();
	d2d::Vector base_t = (base_e - base_s) * process + base_s;
	d2d::Vector pos_t = base_t -  offset;
	tween->setTransform(pos_t, angle);

	float xshear = (end->getShear().x - start->getShear().x) * process + start->getShear().x,
		yshear = (end->getShear().y - start->getShear().y) * process + start->getShear().y;
	tween->setShear(xshear, yshear);

	tween->addCol = cInterpolate(start->addCol, end->addCol, process);
	tween->multiCol = cInterpolate(start->multiCol, end->multiCol, process);

	if (eicon::Sprite* icon_s = dynamic_cast<eicon::Sprite*>(start))
	{
		eicon::Sprite* icon_e = dynamic_cast<eicon::Sprite*>(end);
		eicon::Sprite* icon_t = dynamic_cast<eicon::Sprite*>(tween);
		assert(icon_e && icon_t);
		float proc = (icon_e->GetProcess() - icon_s->GetProcess()) * process + icon_s->GetProcess();
		icon_t->SetProcess(proc);
	}
}

}