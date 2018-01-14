#pragma once

#include <node3/IModel.h>
#include <node3/INode.h>

#include <cu/cu_macro.h>

namespace enode3d
{

class NodeFactory
{
public:
	n3::NodePtr Create(const n3::ModelPtr& model);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}