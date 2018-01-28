#pragma once

#include "node0/NodeComponent.h"

#include <guard/check.h>

namespace n0
{

template <typename T>
bool SceneNode::HasComponent() const 
{
	return m_component_bitset[GetComponentTypeID<T>()];
}

template <typename T, typename... TArgs>
T& SceneNode::AddComponent(TArgs&&... args)
{
	GD_ASSERT(!HasComponent<T>(), "already has the component");

	auto comp_ptr = std::make_unique<T>(std::forward<TArgs>(args)...);
	auto& comp = *comp_ptr;
	size_t idx = m_components.size();
	GD_ASSERT(idx < 256, "too many components");
	m_components.emplace_back(std::move(comp_ptr));

	m_component_array[GetComponentTypeID<T>()]  = static_cast<uint8_t>(idx);
	m_component_bitset[GetComponentTypeID<T>()] = true;

//	comp.Init();
	return comp;
}

template <typename T>
T& SceneNode::GetComponent() const
{
	GD_ASSERT(HasComponent<T>(), "no component");
	auto ptr(m_components[m_component_array[GetComponentTypeID<T>()]].get());
	return *reinterpret_cast<T*>(ptr);
}

}