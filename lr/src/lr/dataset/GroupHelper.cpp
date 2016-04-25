#include "GroupHelper.h"

#include "view/typedef.h"

#include <ee/StringHelper.h>
#include <ee/Matrix.h>

#include <easycomplex.h>

namespace lr
{

ee::Sprite* GroupHelper::Group(const std::vector<ee::Sprite*>& sprites,
							   const std::string& name)
{
	ecomplex::Symbol* sym = new ecomplex::Symbol();
	sym->SetFilepath(GROUP_TAG);
	sym->m_sprites = sprites;
	for_each(sprites.begin(), sprites.end(), ee::RetainObjectFunctor<ee::Sprite>());
	sym->InitBounding();
	ee::Vector c = sym->GetSize().Center();
	for (int i = 0, n = sym->m_sprites.size(); i < n; ++i) {
		sym->m_sprites[i]->Translate(-c);
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

	const ee::Vector& pos = group->GetPosition();
	const ee::Vector& scale = group->GetScale();
	float angle = group->GetAngle();
	for (int i = 0, n = comp->m_sprites.size(); i < n; ++i) 
	{
		ee::Sprite* spr = comp->m_sprites[i]->Clone();

		ee::Vector _scale = spr->GetScale();
		_scale.x *= scale.x;
		_scale.y *= scale.y;

//		float _angle = angle + spr->GetAngle();
		float _angle = angle;

		ee::Vector _pos = spr->GetPosition();
		ee::Matrix mt;
		group->GetTransMatrix(mt);
		_pos = ee::Math2D::TransVector(_pos, mt);

		bool pxmirror, pymirror;
		group->GetMirror(pxmirror, pymirror);

		bool xmirror, ymirror;
		spr->GetMirror(xmirror, ymirror);

		if (pxmirror) {
			xmirror = !xmirror;
		}
		if (pymirror) {
			ymirror= !ymirror;
		}

		if (pxmirror && !pymirror ||
			!pxmirror && pymirror) {
			_angle -= spr->GetAngle();
		} else {
			_angle += spr->GetAngle();
		}

		spr->SetScale(_scale);
		spr->SetTransform(_pos, _angle);
		spr->SetMirror(xmirror, ymirror);

		sprites.push_back(spr);
	}
}

}