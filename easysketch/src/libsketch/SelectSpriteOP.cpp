#include "SelectSpriteOP.h"
#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>

namespace esketch
{

SelectSpriteOP::SelectSpriteOP(StagePanel* stage)
	: ee::EditOP(stage, stage->GetStageImpl())
	, m_stage(stage)
{
	m_selection = stage->GetSpriteSelection();
	m_selection->AddReference();
}

bool SelectSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) return true;

	ee::Sprite* selected = SelectByPos(sm::ivec2(x, y));
	if (selected && selected->IsEditable())
	{
		if (m_stage->GetKeyState(WXK_CONTROL)) 
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

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool SelectSpriteOP::OnDraw() const
{
	if (ee::EditOP::OnDraw()) return true;

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		const Sprite* s = static_cast<const Sprite*>(sprites[i]);
		sm::mat4 mat = sm::mat4(s->GetOri3().ToMatrix()) * 
			sm::mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);
		e3d::DrawCube(mat, s->GetSymbol().GetAABB(), ee::MID_RED);
	}

	return false;
}

// 以sprite的中心和方向，旋转ray的坐标系
// 即AABB不变
ee::Sprite* SelectSpriteOP::SelectByPos(const sm::ivec2& pos) const
{
	ee::Sprite* selected = NULL;

	std::vector<ee::Sprite*> sprites;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	StageCanvas* canvas = static_cast<StageCanvas*>(m_stage->GetCanvas());
	sm::vec3 ray_dir = canvas->TransPos3ScreenToDir(pos);
	e3d::Ray ray(sm::vec3(0, 0, 0), ray_dir);

	sm::mat4 cam_mat = canvas->GetCamera3().GetModelViewMat();
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];
		const Symbol& symbol = static_cast<const Symbol&>(sprite->GetSymbol());
		
		const e3d::AABB& aabb = symbol.GetAABB();
		Sprite* s = static_cast<Sprite*>(sprite);
		
		sm::vec3 offset = cam_mat * s->GetPos3();

		sm::vec3 cross;
		bool intersect = e3d::Math3::RayOBBIntersection(aabb, offset, s->GetOri3(), ray, &cross);
		if (intersect) {
			return sprite;
		}
	}

	return NULL;
}

}