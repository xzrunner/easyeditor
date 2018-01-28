#pragma once

#include <ee/Symbol.h>

#include <node0/SceneNode.h>

#include <cu/cu_macro.h>

namespace ee2
{

class NodeFactory
{
public:
	n0::SceneNodePtr Create(const ee::SymPtr& sym);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}