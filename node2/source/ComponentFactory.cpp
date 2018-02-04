#include "node2/ComponentFactory.h"
#include "node2/CompImage.h"

#include <node0/SceneNode.h>

namespace n2
{

CU_SINGLETON_DEFINITION(ComponentFactory);

ComponentFactory::ComponentFactory()
{
	m_creator.insert(std::make_pair(CompImage::TYPE_NAME,
		[](n0::SceneNodePtr& node, const rapidjson::Value& val)
	{
		auto& comp = node->AddComponent<CompImage>();
		comp.LoadFromJson(val);
	}));
}

void ComponentFactory::Create(n0::SceneNodePtr& node, 
	                          const std::string& name, 
	                          const rapidjson::Value& val)
{
	auto itr = m_creator.find(name);
	if (itr != m_creator.end()) {
		itr->second(node, val);
	} else {
		GD_REPORT_ASSERT("no comp creator");
	}
}

void ComponentFactory::AddCreator(const std::string& name, const CreateFunc& func)
{
	m_creator.insert(std::make_pair(name, func));
}

}