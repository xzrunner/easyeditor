#include "EditUVOP.h"
#include "StagePanel.h"
#include "Mesh.h"
#include "MeshRenderer.h"

#include <sprite2/RenderParams.h>

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
		MeshRenderer::DrawTexture(mesh, s2::RenderParams());
		MeshRenderer::DrawInfoXY(mesh);
	}

	return false;
}

}