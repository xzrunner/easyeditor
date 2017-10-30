#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/Config.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/RenderParams.h>
#include <sprite2/CameraType.h>

namespace emask
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
	, m_mask_render(false)
{
}

void StageCanvas::OnDrawSprites() const
{
	auto& sym = m_stage->GetSymbol();
	if (m_mask_render) {
		sym->DrawTree(nullptr, s2::RenderParams(), NULL);
	} else {
		auto& base = sym->GetBase();
		auto& mask = sym->GetMask();
		if (base) {
			ee::SpriteRenderer::Instance()->Draw(base.get());
		}
		if (mask) {
			ee::SpriteRenderer::Instance()->Draw(mask.get());
		}
	}

	m_stage->DrawEditOP();
}

}