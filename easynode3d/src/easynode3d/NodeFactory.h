#pragma once

#include <sprite2/Symbol.h>
#include <node3/INode.h>

#include <cu/cu_macro.h>


namespace enode3d
{

class NodeFactory
{
public:
	n3::NodePtr Create(const s2::SymPtr& sym);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}