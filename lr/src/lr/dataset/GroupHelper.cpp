#include "GroupHelper.h"

#include "view/typedef.h"

#include <easycomplex.h>

namespace lr
{

d2d::ISprite* GroupHelper::Group(const std::vector<d2d::ISprite*>& sprites)
{
	static int id = 0;
	std::string name = "_group" + d2d::StringTools::ToString(id++);

	ecomplex::Symbol* sym = new ecomplex::Symbol();
	sym->SetFilepath(GROUP_TAG);
	sym->m_sprites = sprites;
	for_each(sprites.begin(), sprites.end(), d2d::RetainObjectFunctor<d2d::ISprite>());
	sym->InitBounding();
	d2d::Vector c = sym->GetSize().Center();
	for (int i = 0, n = sym->m_sprites.size(); i < n; ++i) {
		sym->m_sprites[i]->Translate(-c);
	}
	sym->InitBounding();

	sym->name = name;
	ecomplex::Sprite* spr = new ecomplex::Sprite(sym);
	spr->Translate(c);
	spr->name = name;

	return spr;
}

void GroupHelper::BreakUp(d2d::ISprite* group, std::vector<d2d::ISprite*>& sprites)
{
	ecomplex::Symbol* comp = &dynamic_cast<ecomplex::Symbol&>(const_cast<d2d::ISymbol&>(group->GetSymbol()));
	assert(comp);
	sprites = comp->m_sprites;
	for_each(sprites.begin(), sprites.end(), d2d::RetainObjectFunctor<d2d::ISprite>());

	const d2d::Vector& pos = group->GetPosition();
	const d2d::Vector& scale = group->GetScale();
	float angle = group->GetAngle();
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		d2d::ISprite* spr = sprites[i];

		d2d::Vector _scale = spr->GetScale();
		_scale.x *= scale.x;
		_scale.y *= scale.y;

		float _angle = spr->GetAngle() + angle;

		d2d::Vector _pos = spr->GetPosition();
		d2d::Matrix mt;
		group->GetTransMatrix(mt);
		_pos = d2d::Math::transVector(_pos, mt);

		spr->SetScale(_scale);
		spr->SetTransform(_pos, _angle);
	}
}

}