#include "PackAnimation.h"
#include "type.h"

#include "spritepack.h"

#include "../Exception.h"
#include "../LuaDataHelper.h"
#include "../tools.h"

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

PackAnimation::PackAnimation(lua_State* L, uint32_t id)
	: IPackNode(TYPE_ANIMATION, id)
	, m_clipbox(NULL)
{
}

PackAnimation::~PackAnimation()
{
	delete m_clipbox;
}

size_t PackAnimation::Size() const
{
	size_t sz = 0;

	// components
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		sz += sizeof(m_components[i].id);
		sz += m_components[i].name.Size();
	}

	// actions
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		sz += m_actions[i].name.Size();
		sz += sizeof(m_actions[i].size);
	}

	// frames
	sz += sizeof(uint16_t);
	sz += SIZEOF_ANIMATION 
		+ m_frames.size() * SIZEOF_FRAME
		+ m_actions.size() * SIZEOF_ACTION
		+ m_components.size() * SIZEOF_COMPONENT;
	for (int i = 0, n = m_frames.size(); i < n; ++i) {
		sz += FrameSize(m_frames[i]);
	}

	return sz;
}

void PackAnimation::Store(std::ofstream& fout) const
{
	// components
	uint16_t sz = m_components.size();
	pack2file(sz, fout);
	for (int i = 0; i < sz; ++i) {
		pack2file(m_components[i].id, fout);
		m_components[i].name.Store(fout);
	}

	// actions
	sz = m_actions.size();
	pack2file(sz, fout);
	for (int i = 0; i < sz; ++i) {
		m_actions[i].name.Store(fout);
		pack2file(m_actions[i].size, fout);
	}

	// frames
	sz = m_frames.size();
	pack2file(sz, fout);
	for (int i = 0; i < sz; ++i) {
		StoreFrame(m_frames[i], fout);
	}
}

void PackAnimation::Load(lua_State* L)
{
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
			if (comp.id > m_maxid) {
				m_maxid = comp.id;
			}
		} else {
			comp.id = 0xffff;
			if (!LuaDataHelper::HasField(L, "name")) {
				throw Exception("Anchor need a name");
			}
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

void PackAnimation::LoadFrame(lua_State* L, PackFrame& frame)
{
	int len = lua_rawlen(L, -1);
	frame.parts.reserve(len);

	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		PackPart part;	
		part.touchable = 1;
		if (lua_isnumber(L, -1)) {
			part.only_number = true;
			part.type = TAG_ID;
			part.component_id = lua_tointeger(L, -1);
			if (part.component_id >= m_components.size()) {
				throw Exception("LoadFrame component_id larger than m_components.size()");
			}
		} else {
			part.only_number = false;
			part.type = TAG_ID;
			if (LuaDataHelper::HasField(L, "index")) {
				part.component_id = LuaDataHelper::GetIntField(L, "index");
				if (part.component_id >= m_components.size()) {
					throw Exception("LoadFrame component_id larger than m_components.size()");
				}
			} else {
				throw Exception("LoadFrame component_id null");
			}

			if (LuaDataHelper::HasField(L, "mat")) {
				if (lua_isnumber(L, -1)) {
					part.type |= TAG_MATRIXREF;
					part.t.matref = LuaDataHelper::GetIntField(L, "mat");
				} else if (!IsMatrixIdentity(L)) {
					part.type |= TAG_MATRIX;
					lua_getfield(L, -1, "mat");
					assert(lua_type(L, -1) != LUA_TNIL);
					int len = lua_rawlen(L, -1);
					assert(len == 6);
					for (int i = 1; i <= 6; ++i) {
						lua_pushinteger(L, i);
						lua_gettable(L, -2);
						part.t.mat.m[i-1] = lua_tointeger(L, -1);
						lua_pop(L, 1);
					}
					lua_pop(L, 1);
				}
			}
			if (LuaDataHelper::HasField(L, "color")) {
				uint32_t color = (uint32_t)LuaDataHelper::GetDoubleField(L, "color");
				if (color != 0xffffffff) {
					part.type |= TAG_COLOR;
					part.t.color = color;
				}
			}
			if (LuaDataHelper::HasField(L, "add")) {
				uint32_t add = (uint32_t)LuaDataHelper::GetDoubleField(L, "add");
				if (add != 0) {
					part.type |= TAG_ADDITIVE;
					part.t.additive = add;
				}
			}
			if (LuaDataHelper::HasField(L, "touch")) {
				part.type |= TAG_TOUCH;
			}
			if (LuaDataHelper::HasField(L, "r_map") ||
				LuaDataHelper::HasField(L, "g_map") ||
				LuaDataHelper::HasField(L, "b_map")) {
				part.type |= TAG_COLMAP;
				if (LuaDataHelper::HasField(L, "r_map")) {
					part.t.rmap = (uint32_t)LuaDataHelper::GetDoubleField(L, "r_map");
				} else {
					part.t.rmap = 0xff0000ff;
				}
				if (LuaDataHelper::HasField(L, "g_map")) {
					part.t.gmap = (uint32_t)LuaDataHelper::GetDoubleField(L, "g_map");
				} else {
					part.t.gmap = 0x00ff00ff;
				}
				if (LuaDataHelper::HasField(L, "b_map")) {
					part.t.bmap = (uint32_t)LuaDataHelper::GetDoubleField(L, "b_map");
				} else {
					part.t.bmap = 0x0000ffff;
				}
			}
		}
		frame.parts.push_back(part);

		lua_pop(L, 1);
	}
}

size_t PackAnimation::FrameSize(const PackFrame& frame) const
{
	size_t sz = 0;
	sz += sizeof(uint16_t);
	for (int i = 0, n = frame.parts.size(); i < n; ++i) {
		const PackPart& part = frame.parts[i];
		if (part.only_number) {
			sz += SIZEOF_PART;
		} else {
			sz += SIZEOF_PART + sizeof(Matrix);
		}
	}
	return sz;
}

void PackAnimation::StoreFrame(const PackFrame& frame, std::ofstream& fout) const
{
	uint16_t sz = frame.parts.size();
	pack2file(sz, fout);
	for (int i = 0; i < sz; ++i) {
		const PackPart& part = frame.parts[i];

		pack2file(part.type, fout);
		pack2file(part.component_id, fout);

		if (part.only_number) {
			continue;
		}

		if (part.type & TAG_MATRIXREF) {
			pack2file(part.t.matref, fout);
		} 
		if (part.type & TAG_MATRIX) {
			for (int i = 0; i < 6; ++i) {
				pack2file(part.t.mat.m[i], fout);
			}
		}
		if (part.type & TAG_COLOR) {
			pack2file(part.t.color, fout);
		}
		if (part.type & TAG_ADDITIVE) {
			pack2file(part.t.additive, fout);
		}
		if (part.type & TAG_TOUCH) {
			pack2file(part.touchable, fout);
		}
		if (part.type & TAG_COLMAP) {
			pack2file(part.t.rmap, fout);
			pack2file(part.t.gmap, fout);
			pack2file(part.t.bmap, fout);
		}
	}
}

bool PackAnimation::IsMatrixIdentity(lua_State* L)
{
	if (!LuaDataHelper::HasField(L, "mat")) {
		return false;
	}

	Matrix m;

	lua_getfield(L, -1, "mat");
	assert(lua_type(L, -1) != LUA_TNIL);
	int len = lua_rawlen(L, -1);
	assert(len == 6);
	for (int i = 1; i <= len; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		m.m[i-1] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	return m.m[0] == 1024 && m.m[1] == 0 && m.m[2] == 0
		&& m.m[3] == 1024 && m.m[4] == 0 && m.m[5] == 0;
}

}
}