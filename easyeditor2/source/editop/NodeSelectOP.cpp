#include "ee2/NodeSelectOP.h"
#include "ee2/StagePage.h"
#include "ee2/StageCanvas.h"

#include <ee0/CameraHelper.h>

#include <node2/CompBoundingBox.h>
#include <sprite2/RVG.h>

namespace ee2
{

NodeSelectOP::NodeSelectOP(StagePage& stage)
	: ee0::NodeSelectOP(stage)
{
}

bool NodeSelectOP::OnDraw() const
{
	if (ee0::NodeSelectOP::OnDraw()) {
		return true;
	}

	m_stage.GetNodeSelection().Traverse(
		[](const n0::SceneNodePtr& node)->bool
		{
			CU_VEC<sm::vec2> bound;
			auto& cbb = node->GetComponent<n2::CompBoundingBox>();
			cbb.GetBounding().GetBoundPos(bound);

			s2::RVG::SetColor(s2::Color(255, 0, 0));
			s2::RVG::Polyline(nullptr, bound, true);

			return true;
		}
	);

	return false;
}

n0::SceneNodePtr NodeSelectOP::SelectByPos(int screen_x, int screen_y) const
{
	auto cam = std::dynamic_pointer_cast<StageCanvas>(m_stage.GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*cam, screen_x, screen_y);

	auto& nodes = dynamic_cast<StagePage&>(m_stage).GetAllNodes();
	for (auto& node : nodes)
	{
		auto& cbounding = node->GetComponent<n2::CompBoundingBox>();
		if (cbounding.GetBounding().IsContain(pos)) {
			return node;
		}
	}

	return nullptr;
}

}