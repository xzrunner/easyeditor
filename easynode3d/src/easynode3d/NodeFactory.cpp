#include "NodeFactory.h"

#include <sprite2/SymType.h>
#include <sprite2/ModelSymbol.h>
#include <node3/ModelType.h>
#include <node3/ObjectNode.h>
#include <node3/Sprite2Model.h>
#include <node3/Sprite2Node.h>

namespace enode3d
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

n3::NodePtr NodeFactory::Create(const s2::SymPtr& sym)
{
	if (!sym) {
		return nullptr;
	}

	n3::NodePtr node = nullptr;

	if (sym->Type() == s2::SYM_MODEL)
	{
		auto model_sym = std::dynamic_pointer_cast<s2::ModelSymbol>(sym);
		auto model = model_sym->GetModel();
		int type = model->Type();
		switch (type)
		{
		case n3::MODEL_OBJECT:
			node = std::make_shared<n3::ObjectNode>(model);
			break;
		}
	}
	else
	{
		auto model = std::make_shared<n3::Sprite2Model>(sym);
		node = std::make_shared<n3::Sprite2Node>(model);
	}
	
	return node;
}

}