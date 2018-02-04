#include "ee2/NodeFactory.h"

#include <ee0/CompNodeEditor.h>

#include <sprite2/SymType.h>
#include <sprite2/ImageSymbol.h>
#include <node0/SceneNode.h>
#include <node2/CompImage.h>
#include <node2/CompBoundingBox.h>

namespace ee2
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

n0::SceneNodePtr NodeFactory::Create(const s2::SymPtr& sym)
{
	if (!sym) {
		return nullptr;
	}

	n0::SceneNodePtr node = nullptr;

	if (sym->Type() == s2::SYM_IMAGE)
	{
		auto img_sym = std::dynamic_pointer_cast<s2::ImageSymbol>(sym);

		node = std::make_shared<n0::SceneNode>();

		// image
		auto& cimage = node->AddComponent<n2::CompImage>();
		cimage.SetTexture(img_sym->GetTexture());

		// aabb
		sm::rect sz(img_sym->GetNoTrimedSize().x, img_sym->GetNoTrimedSize().y);
		node->AddComponent<n2::CompBoundingBox>(sz);

		// editor
		node->AddComponent<ee0::CompNodeEditor>();

		auto& tex = img_sym->GetTexture();
	}

	return node;
}

}