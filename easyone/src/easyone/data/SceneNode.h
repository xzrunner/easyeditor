#pragma once

#include <node3/SceneNode.h>

#include <memory>

namespace eone
{

class SceneNode : public n3::SceneNode
{
public:
	
}; // SceneNode

using SceneNodePtr = std::shared_ptr<SceneNode>;

}