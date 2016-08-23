#include "EditUVOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <sprite2/RenderParams.h>
#include <sprite2/DrawMesh.h>

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
		s2::DrawMesh::DrawTexture(mesh, s2::RenderParams());
		s2::DrawMesh::DrawInfoXY(mesh);
	}

	return false;
}

}