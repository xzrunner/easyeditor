#include "EditUVOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/Matrix.h>
#include <ee/RenderParams.h>

namespace emesh
{

EditUVOP::EditUVOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool EditUVOP::OnDraw() const
{
	if (Mesh* mesh = m_stage->GetMesh())
	{
		mesh->DrawTexture(ee::RenderParams());
		mesh->DrawInfoXY();
	}

	return false;
}

}