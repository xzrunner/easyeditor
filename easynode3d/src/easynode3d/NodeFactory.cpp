#include "NodeFactory.h"

#include <node3/ModelType.h>
#include <node3/ObjectNode.h>

namespace enode3d
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

n3::NodePtr NodeFactory::Create(const n3::ModelPtr& model)
{
	if (!model) {
		return nullptr;
	}

	n3::NodePtr node = nullptr;
	int type = model->Type();
	switch (type)
	{
	case n3::MODEL_OBJECT:
		node = std::make_shared<n3::ObjectNode>(model);
		break;
	}
	
	return node;
}

}