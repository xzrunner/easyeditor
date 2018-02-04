#pragma once

#include "node0/NodeComponent.h"

#include <memory>
#include <vector>
#include <bitset>
#include <array>

namespace n0
{

class SceneNode
{
public:
	// tree

	void AddChild(const std::shared_ptr<SceneNode>& child);
	void RemoveChild(const std::shared_ptr<SceneNode>& child);

	const std::vector<std::shared_ptr<SceneNode>>& GetAllChildren() const {
		return m_children;
	}

	void SetParent(const std::shared_ptr<SceneNode>& parent);
	const std::shared_ptr<SceneNode> GetParent() const { return m_parent.lock(); }
	
	// components

	template <typename T>
	bool HasComponent() const;

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args);

	template <typename T>
	T& GetComponent() const;

	const std::vector<std::unique_ptr<NodeComponent>>& GetAllComponents() const {
		return m_components;
	}

private:
	// tree
	std::weak_ptr<SceneNode> m_parent;
	std::vector<std::shared_ptr<SceneNode>> m_children;

	// components
	std::vector<std::unique_ptr<NodeComponent>> m_components;

	static const size_t MAX_COMPONENTS = 32;
	std::bitset<MAX_COMPONENTS>         m_component_bitset;
	std::array<uint8_t, MAX_COMPONENTS> m_component_array;

}; // SceneNode

}

#include "node0/SceneNode.inl"
