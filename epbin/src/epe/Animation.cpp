#include "Animation.h"
#include "type.h"

#include "../Exception.h"
#include "../LuaDataHelper.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <set>
#include <assert.h>

namespace epbin
{
namespace epe
{

Animation::Animation(lua_State* L)
	: INode(TYPE_ANIMATION)
	, m_clipbox(NULL)
{
}

Animation::~Animation()
{
	delete m_clipbox;
}

size_t Animation::Size() const
{
	size_t sz = INode::Size();
	return sz;
}

void Animation::Store(uint8_t** ptr)
{

}

void Animation::Load(lua_State* L)
{
	// export
	std::set<std::string> set_export;
	lua_getfield(L, -1, "export");
	if (lua_type(L, -1) != LUA_TNIL) {
		std::string name = lua_tostring(L, -1);
		std::set<std::string>::iterator itr = set_export.find(name);
		if (itr != set_export.end()) {
			throw Exception("BinaryEPD::BinAni rename:%s\n", name);
		} else {
			set_export.insert(name);
		}
		m_export_name.SetString(name);
	}
	lua_pop(L, 1);	

// 	// clipbox
// 	lua_getfield(L, -1, "clipbox");
// 	if (lua_type(L, -1) != LUA_TNIL) {
// 		m_type = CLIPUI;
// 		m_clipbox = new Clipbox;
// 		int len = lua_rawlen(L, -1);
// 		assert(len == 4);
// 		for (int i = 1; i <= len; ++i) {
// 			lua_pushinteger(L, i);
// 			lua_gettable(L, -2);
// 			m_clipbox->cb[i-1] = lua_tointeger(L, -1);
// 			lua_pop(L, 1);
// 		}
// 	}
// 	lua_pop(L, 1);

	// components
	lua_getfield(L, -1, "component");
	int len = lua_rawlen(L, -1);
	m_components.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);

		Component comp;
		if (LuaDataHelper::HasField(L, "id")) {
			comp.id = LuaDataHelper::GetIntField(L, "id");
		}
		if (LuaDataHelper::HasField(L, "name")) {
			comp.name = LuaDataHelper::GetStringField(L, "name");
		}
		m_components.push_back(comp);

		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	int frame_count = 0;
	// actions
	len = lua_rawlen(L, -1);
	m_actions.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Action action;
		if (LuaDataHelper::HasField(L, "action")) {
			action.name = LuaDataHelper::GetStringField(L, "action");
		}
		action.size = lua_rawlen(L, -1);

		frame_count += action.size;

		m_actions.push_back(action);

		lua_pop(L, 1);
	}

	// frames
	m_frames.reserve(frame_count);	
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		PackFrame frame;
		LoadFrame(L, frame);	
		m_frames.push_back(frame);

		lua_pop(L, 1);
	}
}

void Animation::LoadFrame(lua_State* L, PackFrame& frame)
{
	int len = lua_rawlen(L, -1);
	frame.parts.reserve(len);

	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		PackPart part;
		

		frame.parts.push_back(part);

		lua_pop(L, 1);
	}
}

}
}