#pragma once

#include <rapidjson/document.h>

namespace n0
{

using ComponentID = size_t;

namespace Internal
{
inline ComponentID GetUniqueComponentID() noexcept
{
    static ComponentID id{0u};
    return id++;
}
}

template <typename T>
inline ComponentID GetComponentTypeID() noexcept
{
    static_assert(std::is_base_of<NodeComponent, T>::value,
        "T must inherit from Component");

    static ComponentID type_id{Internal::GetUniqueComponentID()};
    return type_id;
}

class NodeComponent
{
public:
	virtual ~NodeComponent() {}

	//virtual void Init() {}
	//virtual void Update() {}
	//virtual void Draw() {}

	virtual const char* Type() const = 0;

	virtual bool StoreToJson(rapidjson::Value& val, 
		rapidjson::MemoryPoolAllocator<>& alloc) const { return false; }
	virtual void LoadFromJson(const rapidjson::Value& val) {}

}; // NodeComponent

}