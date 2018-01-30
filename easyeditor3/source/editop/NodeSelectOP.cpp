#include "ee3/NodeSelectOP.h"
#include "ee3/StagePage.h"
#include "ee3/StageCanvas.h"

#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>

#include <ee0/MessageID.h>

#include <guard/check.h>
#include <node3/PrimitiveDraw.h>
#include <node3/Ray.h>
#include <node3/Math.h>
#include <node3/CompAABB.h>
#include <node3/CompTransform.h>

namespace ee3
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
			n3::PrimitiveDraw::SetColor(ee::MID_RED.ToABGR());

			auto& caabb = node->GetComponent<n3::CompAABB>();
			auto& ctrans = node->GetComponent<n3::CompTransform>();

			sm::mat4 prev_mt;
			auto parent = node->GetParent();
			while (parent)
			{
				auto& pctrans = parent->GetComponent<n3::CompTransform>();
				prev_mt = pctrans.GetTransformMat() * prev_mt;
				parent = parent->GetParent();
			}

			n3::PrimitiveDraw::Cube(prev_mt * ctrans.GetTransformMat(), caabb.GetAABB());

			return true;
		}
	);

	return false;
}

// AABB not changed, transform ray from Camera and spr's mat
n0::SceneNodePtr NodeSelectOP::SelectByPos(int screen_x, int screen_y) const
{
	auto& nodes = dynamic_cast<StagePage&>(m_stage).GetAllNodes();

	auto canvas = std::dynamic_pointer_cast<StageCanvas>(m_stage.GetCanvas());
	auto& vp = canvas->GetViewport();
	auto& cam = canvas->GetCamera();
	sm::vec3 ray_dir = vp.TransPos3ScreenToDir(sm::vec2(screen_x, screen_y), cam);
	n3::Ray ray(cam.GetPos(), ray_dir);
	for (auto& node : nodes)
	{
		auto& caabb = node->GetComponent<n3::CompAABB>();
		auto& ctrans = node->GetComponent<n3::CompTransform>();

		sm::vec3 cross;
		bool intersect = n3::Math::RayOBBIntersection(
			caabb.GetAABB(),
			ctrans.GetPosition(), 
			ctrans.GetAngle(), 
			ray, 
			&cross);
		if (intersect) {
			return node;
		}
	}

	return nullptr;
}

}