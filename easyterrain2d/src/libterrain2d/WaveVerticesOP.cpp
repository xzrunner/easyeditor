#include "WaveVerticesOP.h"
#include "StagePanel.h"
#include "OceanMesh.h"

namespace eterrain2d
{

WaveVerticesOP::WaveVerticesOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true, false)
	, m_stage(stage)
	, m_draw_tris(false)
{
}

bool WaveVerticesOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) {
		return true;
	}

	const std::vector<OceanMesh*>& oceans = m_stage->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Draw(d2d::Matrix(), d2d::Colorf(1, 1, 1, 1), 
			d2d::Colorf(0, 0, 0, 0), m_draw_tris);
	}

	return false;
}

}