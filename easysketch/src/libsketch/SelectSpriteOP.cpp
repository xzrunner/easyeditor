#include "SelectSpriteOP.h"
#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Sprite.h"

namespace libsketch
{

SelectSpriteOP::SelectSpriteOP(StagePanel* stage)
	: d2d::AbstractEditOP(stage)
	, m_stage(stage)
{
	m_selection = stage->getSpriteSelection();
	m_selection->Retain();
}

bool SelectSpriteOP::onMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseLeftDown(x, y)) return true;

	d2d::ISprite* selected = SelectByPos(ivec2(x, y));
	if (selected && selected->editable)
	{
		if (wxGetKeyState(WXK_CONTROL)) 
		{
			if (m_selection->IsExist(selected)) {
				m_selection->Remove(selected);
			} else {
				m_selection->Add(selected);
			}
		}
		else
		{
			if (!m_selection->IsExist(selected))
			{
				m_selection->Clear();
				m_selection->Add(selected);
			}
		}
	}
	else
	{
		m_selection->Clear();
	}

	return false;
}

bool SelectSpriteOP::onDraw() const
{
	if (d2d::AbstractEditOP::onDraw()) return true;

	std::vector<d2d::ISprite*> sprites;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		const Sprite* s = static_cast<const Sprite*>(sprites[i]);

		e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();

		mat4 mat = mat4(s->GetOri3().ToMatrix()) * 
			mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);

		e3d::DrawCube(mat, s->getSymbol().GetAABB(), d2d::MID_RED);
	}

	m_stage->Refresh();

	return false;
}

// 以sprite的中心和方向，旋转ray的坐标系
// 即AABB不变
d2d::ISprite* SelectSpriteOP::SelectByPos(const ivec2& pos) const
{
	d2d::ISprite* selected = NULL;

	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	StageCanvas* canvas = static_cast<StageCanvas*>(m_editPanel->getCanvas());
	vec3 ray_dir = canvas->TransPos3ScreenToDir(pos);
	e3d::Ray ray(vec3(0, 0, 0), ray_dir);

	mat4 cam_mat = canvas->GetCamera3().GetModelViewMat();
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		const Symbol& symbol = static_cast<const Symbol&>(sprite->getSymbol());
		
		const e3d::AABB& aabb = symbol.GetAABB();
		Sprite* s = static_cast<Sprite*>(sprite);
		
		vec3 offset = cam_mat * s->GetPos3();

		vec3 cross;
		bool intersect = e3d::Math3::RayOBBIntersection(aabb, offset, s->GetOri3(), ray, &cross);
		if (intersect) {
			return sprite;
		}
	}

	return NULL;
}

}