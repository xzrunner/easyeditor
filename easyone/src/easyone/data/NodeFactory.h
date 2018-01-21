#pragma once

#include "data/SceneNode.h"

#include <ee/Symbol.h>

#include <cu/cu_macro.h>

namespace eone
{

class NodeFactory
{
public:
	SceneNodePtr Create(const ee::SymPtr& sym);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}