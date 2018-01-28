#include "ee2/NodeFactory.h"
#include "ee2/CompEditor.h"

#include <sprite2/SymType.h>
#include <sprite2/ImageSymbol.h>
#include <node2/CompImage.h>

namespace ee2
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

n0::SceneNodePtr NodeFactory::Create(const ee::SymPtr& sym)
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

		// editor
		node->AddComponent<CompEditor>();

		auto& tex = img_sym->GetTexture();
	}

	return node;
}

}