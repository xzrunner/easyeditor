#include "ee3/NodeFactory.h"
#include "ee3/CompEditor.h"

#include <sprite2/SymType.h>
#include <sprite2/ModelSymbol.h>
#include <node3/CompModel.h>
#include <node3/CompAABB.h>

namespace ee3
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

n3::SceneNodePtr NodeFactory::Create(const ee::SymPtr& sym)
{
	if (!sym) {
		return nullptr;
	}

	n3::SceneNodePtr node = nullptr;

	if (sym->Type() == s2::SYM_MODEL)
	{
		auto model_sym = std::dynamic_pointer_cast<s2::ModelSymbol>(sym);
		auto& src_node = model_sym->GetNode();
		if (src_node)
		{
			node = std::make_shared<n3::SceneNode>();

			// model
			auto& cmodel = node->AddComponent<n3::CompModel>();
			auto& src_model = src_node->GetComponent<n3::CompModel>();
			cmodel.SetModel(src_model.GetModel());

			// aabb
			auto& caabb = node->AddComponent<n3::CompAABB>();
			auto& src_aabb = src_node->GetComponent<n3::CompAABB>();
			caabb.SetAABB(src_aabb.GetAABB());

			// editor
			node->AddComponent<CompEditor>();
		}
	}

	return node;
}

}