#include "data/NodeFactory.h"

#include <sprite2/SymType.h>
#include <sprite2/ModelSymbol.h>
#include <node3/CompModel.h>
#include <node3/CompAABB.h>
#include <node3/ModelType.h>
#include <node3/ObjectModel.h>

namespace eone
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

SceneNodePtr NodeFactory::Create(const ee::SymPtr& sym)
{
	if (!sym) {
		return nullptr;
	}

	SceneNodePtr node = nullptr;

	if (sym->Type() == s2::SYM_MODEL)
	{
		auto model_sym = std::dynamic_pointer_cast<s2::ModelSymbol>(sym);
		auto model = model_sym->GetModel();
		if (model->Type() == n3::MODEL_OBJECT)
		{
			auto m = std::dynamic_pointer_cast<n3::ObjectModel>(model)->GetModel();

			node = std::make_shared<SceneNode>();

			// model
			auto& cmodel = node->AddComponent<n3::CompModel>();
			cmodel.SetModel(m);

			// aabb
			auto& caabb = node->AddComponent<n3::CompAABB>();
			caabb.SetAABB(model->GetAABB());
		}
	}

	return node;
}

}