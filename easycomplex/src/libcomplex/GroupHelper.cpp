#include "GroupHelper.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/Math2D.h>
#include <ee/SpriteFactory.h>

#include <sprite2/S2_Sprite.h>

#include <assert.h>

namespace ecomplex
{

Sprite* GroupHelper::Group(const std::vector<ee::Sprite*>& sprs)
{
	Symbol* sym = new Symbol();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sym->Add(sprs[i]);
	}

	sm::vec2 c = sym->GetBounding().Center();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->Translate(-c);
	}

	Sprite* spr = new Sprite(sym);
	spr->Translate(c);
	ee::SpriteFactory::Instance()->Insert(spr);

	return spr;
}

void GroupHelper::BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprs)
{
	Symbol* comp = dynamic_cast<Symbol*>(group->GetSymbol());
	assert(comp);

	const sm::vec2& pos = group->GetPosition();
	const sm::vec2& scale = group->GetScale();
	float angle = group->GetAngle();
	const std::vector<s2::Sprite*>& children = comp->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)children[i])->Clone());

		sm::vec2 _scale = spr->GetScale();
		_scale.x *= scale.x;
		_scale.y *= scale.y;

//		float _angle = angle + spr->GetAngle();
		float _angle = angle;

		sm::vec2 _pos = group->GetTransMatrix() * spr->GetPosition();

		const sm::vec2& gs = group->GetScale();
		if (gs.x < 0 && gs.x >= 0 ||
			gs.y >= 0 && gs.y < 0) {
		} else {
			_angle += spr->GetAngle();
		}

		spr->SetScale(_scale);
		spr->SetAngle(_angle);
		spr->SetPosition(_pos);

		sprs.push_back(spr);
	}
}

}