#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/cfg_const.h>
#include <ee/Config.h>

#include <ps_3d.h>
#include <shaderlab/Statistics.h>
#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>
#include <sprite2/OrthoCamera.h>
#include <gum/GTxt.h>
#include <gum/StringHelper.h>

namespace eparticle3d
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(editPanel)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	int num = sl::Statistics::Instance()->GetVertices();

	DrawBackground();

	std::vector<ee::SprPtr> sprs;
	static_cast<StagePanel*>(m_stage)->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];
		if (!spr->IsVisible())
			continue;
		ee::SpriteRenderer::Instance()->Draw(sprs[i].get());
	}

	ParticleSystem* ps = m_stage->m_ps;
	if (ps) {
		m_stage->m_ps->Draw();
	}

	m_stage->DrawEditOP();

	if (ee::Config::Instance()->IsDrawStatOpen()) {
		DrawStat();
	}
}

void StageCanvas::DrawBackground() const
{
	s2::RVG::SetColor(s2::Color(204, 204, 204));
	s2::RVG::LineWidth(2);
	s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

void StageCanvas::DrawStat() const
{
	s2::OrthoCamera* cam = static_cast<s2::OrthoCamera*>(m_camera);	
	S2_MAT mt;
	float s = std::max(1.0f, cam->GetScale());
	mt.Scale(s, s);
	mt.Translate(cam->GetPosition().x - 200, cam->GetPosition().y + 200);

	int num = sl::Statistics::Instance()->GetVertices() / 6;

	gum::GTxt::Instance()->Draw(mt, gum::StringHelper::ToString(num));

	sl::Statistics::Instance()->Reset();
}

}