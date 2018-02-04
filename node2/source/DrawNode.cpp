#include "node2/DrawNode.h"
#include "node2/CompTransform.h"
#include "node2/CompImage.h"

#include <node0/SceneNode.h>
#include <painting2/RenderSystem.h>

namespace n2
{

void DrawNode::Draw(const n0::SceneNodePtr& node, const N2_MAT& mt)
{
	auto& ctrans = node->GetComponent<CompTransform>();
	auto mt_child = ctrans.GetTrans().GetMatrix() * mt;

	if (node->HasComponent<CompImage>())
	{
		auto& cimage = node->GetComponent<CompImage>();
		auto& tex = cimage.GetTexture();
		if (tex) {
			auto sz = tex->GetSize();
			pt2::RenderSystem::DrawTexture(*tex, sm::rect(sz.x, sz.y), mt_child);
		}
	}

	auto& children = node->GetAllChildren();
	for (auto& child : children) {
		Draw(child, mt_child);
	}
}

}