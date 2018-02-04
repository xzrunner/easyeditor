#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <rapidjson/document.h>

#include <functional>
#include <map>

namespace n2
{

class ComponentFactory
{
public:
	using CreateFunc = std::function<void(n0::SceneNodePtr&, const rapidjson::Value&)>;

	void Create(n0::SceneNodePtr& node, const std::string& name,
		const rapidjson::Value& val);

	void AddCreator(const std::string& name, const CreateFunc& func);

private:
	std::map<std::string, CreateFunc> m_creator;

	CU_SINGLETON_DECLARATION(ComponentFactory);

}; // ComponentFactory

}