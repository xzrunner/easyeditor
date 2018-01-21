#pragma once

#include <node3/INode.h>

namespace enode3d
{

class PropertySetting;

class Node : public n3::INode
{
public:
	virtual PropertySetting* CreatePropertySetting();

}; // Node

using NodePtr = std::shared_ptr<Node>;

}