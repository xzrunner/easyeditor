#pragma once

#include <node3/SceneNode.h>

#include <memory>

namespace eone
{

class SceneNode : public n3::SceneNode
{
public:
	SceneNode();

	const std::string& GetName() const;
	void SetName(const std::string& name);
	
private:
	std::string m_name;

}; // SceneNode

using SceneNodePtr = std::shared_ptr<SceneNode>;

}