#include "WaveVerticesOP.h"
#include "StagePanel.h"
#include "OceanMesh.h"

#include <sprite2/RenderParams.h>

namespace eterrain2d
{

WaveVerticesOP::WaveVerticesOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true, false)
	, m_stage(stage)
	, m_draw_tris(false)
{
}

bool WaveVerticesOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) {
		return true;
	}

	const std::vector<OceanMesh*>& oceans = m_stage->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Draw(s2::RenderParams(), m_draw_tris);
	}

	return false;
}

}