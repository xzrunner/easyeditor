#pragma once

#include <ee/Symbol.h>

#include <node3/SceneNode.h>

#include <cu/cu_macro.h>

namespace ee3
{

class NodeFactory
{
public:
	n3::SceneNodePtr Create(const ee::SymPtr& sym);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}