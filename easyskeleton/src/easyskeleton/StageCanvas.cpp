#include "StageCanvas.h"
#include "StagePanel.h"
#include "Bone.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/S2_RVG.h>

namespace eskeleton
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), gum::CAM_ORTHO2D), m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	std::vector<ee::Sprite*> sprs;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* spr = sprs[i];
		ee::SpriteRenderer::Instance()->Draw(spr);
		Bone* bone = (Bone*)(spr->GetUserData());
		bone->Draw();
	}
	
	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	s2::RVG::SetColor(s2::Color(204, 204, 204));
	s2::RVG::LineWidth(2);
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);
}

}