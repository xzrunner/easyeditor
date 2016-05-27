#include "GroupHelper.h"

#include "view/typedef.h"

#include <ee/StringHelper.h>
#include <ee/Math2D.h>

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

namespace lr
{

ee::Sprite* GroupHelper::Group(const std::vector<ee::Sprite*>& sprites,
							   const std::string& name)
{
	ecomplex::Symbol* sym = new ecomplex::Symbol();
	sym->SetFilepath(GROUP_TAG);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Retain();
		sym->Add(sprites[i]);
	}

	sym->InitBounding();
	sm::vec2 c = sym->GetSize().Center();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Translate(-c);
	}
	sym->InitBounding();

	sym->name = name;
	sym->SetName(name);
	ecomplex::Sprite* spr = new ecomplex::Sprite(sym);
	spr->Translate(c);
	spr->name = name;

	return spr;
}

void GroupHelper::BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprites)
{
	ecomplex::Symbol* comp = &dynamic_cast<ecomplex::Symbol&>(const_cast<ee::Symbol&>(group->GetSymbol()));
	assert(comp);

	const sm::vec2& pos = group->GetPosition();
	const sm::vec2& scale = group->GetScale();
	float angle = group->GetAngle();
	const std::vector<s2::Sprite*>& children = comp->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		ee::Sprite* spr = static_cast<ee::Sprite*>(children[i]->GetUD())->Clone();

		sm::vec2 _scale = spr->GetScale();
		_scale.x *= scale.x;
		_scale.y *= scale.y;

//		float _angle = angle + spr->GetAngle();
		float _angle = angle;

		sm::vec2 _pos = spr->GetPosition();
		sm::mat4 mt;
		group->GetTransMatrix(mt);
		_pos = ee::Math2D::TransVector(_pos, mt);

		sm::bvec2 pmirror = group->GetMirror(),
			mirror = spr->GetMirror();

		if (pmirror.x) {
			mirror.x = !mirror.x;
		}
		if (pmirror.y) {
			mirror.y = !mirror.y;
		}

		if (pmirror.x && !pmirror.y ||
			!pmirror.x && pmirror.y) {
			_angle -= spr->GetAngle();
		} else {
			_angle += spr->GetAngle();
		}

		spr->SetScale(_scale);
		spr->SetTransform(_pos, _angle);
		spr->SetMirror(mirror.x, mirror.y);

		sprites.push_back(spr);
	}
}

}