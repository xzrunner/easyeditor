#include "SprSelectOP.h"
#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>

#include <node3/PrimitiveDraw.h>
#include <node3/Ray.h>
#include <node3/Math.h>
#include <sprite2/ModelSprite.h>
#include <sprite2/ModelSymbol.h>

namespace enode3d
{

SprSelectOP::SprSelectOP(StagePanel& stage)
	: ee::EditOP(&stage, stage.GetStageImpl())
	, m_stage(stage)
{
	m_selection = stage.GetSpriteSelection();
	m_selection->AddReference();
}

bool SprSelectOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) return true;

	ee::SprPtr selected = SelectByPos(sm::vec2(x, y));
	if (selected && selected->IsEditable())
	{
		if (m_stage.GetKeyState(WXK_CONTROL)) 
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

bool SprSelectOP::OnDraw() const
{
	if (ee::EditOP::OnDraw()) return true;

	std::vector<ee::SprPtr> sprs;
	m_selection->Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (auto& spr : sprs) 
	{
		auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(spr);
		auto pos = model_spr->GetPos3();
		sm::mat4 mat = sm::mat4(model_spr->GetOri3()) *
			sm::mat4::Translated(pos.x, pos.y, pos.z);
		n3::PrimitiveDraw::SetColor(ee::MID_RED.ToABGR());
		// todo mat
		n3::PrimitiveDraw::Cube(std::dynamic_pointer_cast<s2::ModelSymbol>(
			model_spr->GetSymbol())->GetAABB());
	}

	return false;
}

// 以sprite的中心和方向，旋转ray的坐标系
// 即AABB不变
ee::SprPtr SprSelectOP::SelectByPos(const sm::vec2& pos) const
{
	ee::SprPtr selected = NULL;

	std::vector<ee::SprPtr> sprs;
	m_stage.TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));

	StageCanvas* canvas = static_cast<StageCanvas*>(m_stage.GetCanvas());
	sm::vec3 ray_dir = canvas->TransPos3ScreenToDir(pos);
	n3::Ray ray(sm::vec3(0, 0, 0), ray_dir);

	sm::mat4 cam_mat = canvas->GetCamera().GetModelViewMat();
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];
		auto sym = std::dynamic_pointer_cast<const s2::ModelSymbol>(spr->GetSymbol());
		
		const n3::AABB& aabb = sym->GetAABB();
		auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(spr);
		
		sm::vec3 offset = cam_mat * model_spr->GetPos3();

		sm::vec3 cross;
		bool intersect = n3::Math::RayOBBIntersection(aabb, offset, model_spr->GetOri3(), ray, &cross);
		if (intersect) {
			return spr;
		}
	}

	return NULL;
}

}