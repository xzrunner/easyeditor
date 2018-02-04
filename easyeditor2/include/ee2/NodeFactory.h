#pragma once

#include <sprite2/typedef.h>
#include <node0/typedef.h>

#include <cu/cu_macro.h>

namespace ee2
{

class NodeFactory
{
public:
	n0::SceneNodePtr Create(const s2::SymPtr& sym);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}