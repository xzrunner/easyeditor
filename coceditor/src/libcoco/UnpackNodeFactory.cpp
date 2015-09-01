#include "UnpackNodeFactory.h"

#include "PackPicture.h"
#include "PackLabel.h"
#include "PackAnimation.h"

#include <epbin.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

namespace libcoco
{

UnpackNodeFactory* UnpackNodeFactory::m_instance = NULL;

UnpackNodeFactory::UnpackNodeFactory()
{
}

void UnpackNodeFactory::Unpack(lua_State* L, const std::vector<d2d::Image*>& images)
{
	std::string type = epbin::LuaDataHelper::GetStringField(L, "type");
	int id = epbin::LuaDataHelper::GetIntField(L, "id");

	IPackNode* node = NULL;
	if (type == "picture") {
		node = new PackPicture;
	} else if (type == "label") {
		node = new PackLabel;
	} else if (type == "animation") {
		node = new PackAnimation;
	} else {
		throw d2d::Exception("UnpackNodeFactory::Unpack unknown type %s", type.c_str());
	}

	node->UnpackFromLua(L, images);

	m_map_id.insert(std::make_pair(id, node));

	if (type == "animation") {
		PackAnimation* anim = static_cast<PackAnimation*>(node);
		if (!anim->export_name.empty()) {
			std::map<std::string, IPackNode*>::iterator itr
				= m_map_name.find(anim->export_name);
			if (itr != m_map_name.end()) {
				throw d2d::Exception("duplicate export name %s", anim->export_name);
			} else {
				m_map_name.insert(std::make_pair(anim->export_name, node));
			}
		}
	}
}

void UnpackNodeFactory::Query(int id, const IPackNode** ret) const
{
	std::map<int, IPackNode*>::const_iterator itr 
		= m_map_id.find(id);
	if (itr != m_map_id.end()) {
		*ret = itr->second;
	} else {
		m_unassigned.push_back(std::make_pair(id, ret));
	}
}

void UnpackNodeFactory::AfterUnpack()
{
	for (int i = 0, n = m_unassigned.size(); i < n; ++i) {
		int id = m_unassigned[i].first;
		std::map<int, IPackNode*>::iterator itr = m_map_id.find(id);
		assert(itr != m_map_id.end());
		*(m_unassigned[i].second) = itr->second;
	}
	m_unassigned.clear();
}

UnpackNodeFactory* UnpackNodeFactory::Instance()
{
	if (!m_instance) {
		m_instance = new UnpackNodeFactory;
	}
	return m_instance;
}

}