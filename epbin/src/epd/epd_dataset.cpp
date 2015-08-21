#include "epd_dataset.h"

#include "../tools.h"
#include "../LuaDataHelper.h"
#include "../Exception.h"

#include <algorithm>
#include <assert.h>
//#include <math.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

namespace epbin
{
namespace epd
{

static const int ANIMATION	= 0;
static const int PICTURE	= 1;
static const int CLIPUI		= 2;

static const int COMPONENT	= 0;
static const int SWITCH		= 1;
static const int LABEL		= 2;
static const int MOUNT		= 3;

static const int FID		= 0;
static const int FCOLOR		= 1;
static const int FMAT		= 2;
static const int FCLIP		= 4;

//////////////////////////////////////////////////////////////////////////
// class Picture
//////////////////////////////////////////////////////////////////////////

Picture::Picture(lua_State* L, int id)
	: m_id(id)
{	
	int len = lua_rawlen(L, -1);
	m_parts.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Picture::Part part;
		// tex
		lua_getfield(L, -1, "tex");
		const char* type = lua_typename(L, lua_type(L, -1));
		part.tex = (uint8_t)lua_tointeger(L, -1);
		lua_pop(L, 1);
		// src
		lua_getfield(L, -1, "src");
		int len = lua_rawlen(L, -1);
		assert(len == 8);
		uint16_t* src = &part.src[0];
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			src[i-1] = (uint16_t)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		// screen
		lua_getfield(L, -1, "screen");
		len = lua_rawlen(L, -1);
		assert(len == 8);
		int32_t* dst = &part.screen[0];
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);

// 			double v = lua_tonumber(L, -1);
// 			v -= 0.00025;
// 			dst[i-1] = floor(v + 0.5);

			dst[i-1] = lua_tointeger(L, -1);

			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		m_parts.push_back(part);

		lua_pop(L, 1);
	}
}

size_t Picture::Size() const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);					// type
	sz += sizeof(m_id);						// id
	sz += sizeof(uint16_t);					// size
	for (int i = 0, n = m_parts.size(); i < n; ++i) {
		sz += m_parts[i].Size();
	}
	return sz;
}

void Picture::Store(uint8_t** ptr)
{
	memcpy(*ptr, &PICTURE, sizeof(uint8_t));
	*ptr += sizeof(uint8_t);

	pack2mem(m_id, ptr);

	uint16_t sz = m_parts.size();
	pack2mem(sz, ptr);
	for (size_t i = 0; i < sz; ++i) {
		m_parts[i].Store(ptr);
	}
}

//////////////////////////////////////////////////////////////////////////
// class Picture::Part
//////////////////////////////////////////////////////////////////////////

size_t Picture::Part::Size() const
{
	return sizeof(tex) + sizeof(src) + sizeof(screen);
}

void Picture::Part::Store(uint8_t** ptr)
{
	pack2mem(tex, ptr);
	for (int i = 0; i < 8; ++i) {
		pack2mem(src[i], ptr);
	}
	for (int i = 0; i < 8; ++i) {
		pack2mem(screen[i], ptr);
	}
}

//////////////////////////////////////////////////////////////////////////
// class Component
//////////////////////////////////////////////////////////////////////////

Component::Component(lua_State* L)
	: IPackNode(COMPONENT)
{
	m_id = LuaDataHelper::GetIntField(L, "id");
}

size_t Component::Size() const
{
	return sizeof(uint8_t) + sizeof(m_id);
}

void Component::Store(uint8_t** ptr) const
{
	pack2mem(GetType(), ptr);
	pack2mem(m_id, ptr);
}

//////////////////////////////////////////////////////////////////////////
// class Switch
//////////////////////////////////////////////////////////////////////////

Switch::Switch(lua_State* L)
	: IPackNode(SWITCH)
{
	lua_getfield(L, -1, "name");
	if (lua_type(L, -1) != LUA_TNIL) {
		m_name.SetString(lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	m_id = LuaDataHelper::GetIntField(L, "id");
}

size_t Switch::Size() const
{
	size_t sz = sizeof(uint8_t);
	sz += sizeof(m_id);
	sz += m_name.Size();
	return sz;
}

void Switch::Store(uint8_t** ptr) const
{
	pack2mem(GetType(), ptr);
	pack2mem(m_id, ptr);
	m_name.Store(ptr);
}

//////////////////////////////////////////////////////////////////////////
// class Label
//////////////////////////////////////////////////////////////////////////

Label::Label(lua_State* L)
	: IPackNode(LABEL)
{
	lua_getfield(L, -1, "name");
	if (lua_type(L, -1) != LUA_TNIL) {
		m_name.SetString(lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "font");
	if (lua_type(L, -1) != LUA_TNIL) {
		m_font.SetString(lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	m_color = (uint32_t)LuaDataHelper::GetDoubleField(L, "color");
	m_size = LuaDataHelper::GetIntField(L, "size");
	m_align = LuaDataHelper::GetIntField(L, "align");
	m_width = LuaDataHelper::GetIntField(L, "width");
	m_height = LuaDataHelper::GetIntField(L, "height");
}

size_t Label::Size() const
{
	size_t sz = sizeof(uint8_t);
	sz += m_name.Size();
	sz += m_font.Size();
	sz += sizeof(m_color);
	sz += sizeof(m_size);
	sz += sizeof(m_align);
	sz += sizeof(m_width);
	sz += sizeof(m_height);
	return sz;
}

void Label::Store(uint8_t** ptr) const
{
	pack2mem(GetType(), ptr);

	m_name.Store(ptr);
	m_font.Store(ptr);

	pack2mem(m_color, ptr);
	pack2mem(m_size, ptr);
	pack2mem(m_align, ptr);
	pack2mem(m_width, ptr);
	pack2mem(m_height, ptr);
}

//////////////////////////////////////////////////////////////////////////
// class Mount
//////////////////////////////////////////////////////////////////////////

Mount::Mount(lua_State* L)
: IPackNode(MOUNT)
{
	lua_getfield(L, -1, "name");
	if (lua_type(L, -1) != LUA_TNIL) {
		m_name.SetString(lua_tostring(L, -1));
	}
	lua_pop(L, 1);
}

size_t Mount::Size() const
{
	size_t sz = sizeof(uint8_t);
	sz += m_name.Size();
	return sz;
}

void Mount::Store(uint8_t** ptr) const
{
	pack2mem(GetType(), ptr);
	m_name.Store(ptr);
}

//////////////////////////////////////////////////////////////////////////
// class Sprite
//////////////////////////////////////////////////////////////////////////

Sprite::Sprite(lua_State* L, int max_idx)
	: m_color(NULL)
	, m_add(NULL)
	, m_mat(NULL)
{
	if (lua_isnumber(L, -1))
	{
		m_type = FID;
		m_id = lua_tointeger(L, -1);
		if (m_id >= max_idx) {
			throw Exception("Sprite::Sprite id larger than max");
		}
	}
	else
	{
		m_id = LuaDataHelper::GetIntField(L, "index");
		if (m_id >= max_idx) {
			throw Exception("Sprite::Sprite id larger than max");
		}

		if (LuaDataHelper::HasField(L, "mat")) {
			m_mat = new Matrix;
			lua_getfield(L, -1, "mat");
			assert(lua_type(L, -1) != LUA_TNIL);
			int len = lua_rawlen(L, -1);
			assert(len == 6);
			for (int i = 1; i <= len; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				m_mat->m[i-1] = lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		} else if (LuaDataHelper::HasField(L, "scale") || LuaDataHelper::HasField(L, "trans")) {
			m_mat = new Matrix;
			if (LuaDataHelper::HasField(L, "scale")) {
				lua_getfield(L, -1, "scale");
				assert(lua_type(L, -1) != LUA_TNIL);
				int len = lua_rawlen(L, -1);
				assert(len == 2);

				lua_pushinteger(L, 1);
				lua_gettable(L, -2);
				double sx = lua_tonumber(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				double sy = lua_tonumber(L, -1);
				lua_pop(L, 1);

				m_mat->m[0] *= (int)sx;
				m_mat->m[1] *= (int)sy;
				m_mat->m[2] *= (int)sx;
				m_mat->m[3] *= (int)sy;
				m_mat->m[4] *= (int)sx;
				m_mat->m[5] *= (int)sy;

				lua_pop(L, 1);
			}
			if (LuaDataHelper::HasField(L, "trans")) {
				lua_getfield(L, -1, "trans");
				assert(lua_type(L, -1) != LUA_TNIL);
				int len = lua_rawlen(L, -1);
				assert(len == 2);

				lua_pushinteger(L, 1);
				lua_gettable(L, -2);
				int tx = lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				int ty = lua_tointeger(L, -1);
				lua_pop(L, 1);

				m_mat->m[4] += tx;
				m_mat->m[5] += ty;

				lua_pop(L, 1);
			}
		}

		if (LuaDataHelper::HasField(L, "color")) {
			m_color = new uint32_t;
			*m_color = (uint32_t)LuaDataHelper::GetDoubleField(L, "color");
			m_add = new uint32_t;
			*m_add = (uint32_t)LuaDataHelper::GetDoubleField(L, "add");
		}

		m_type = FID;
		if (LuaDataHelper::HasField(L, "clip")) {
			m_type += FCLIP;
		}
		if (m_color) {
			m_type += FCOLOR;
		}
		if (m_mat) {
			m_type += FMAT;
		}
	}
}

Sprite::~Sprite()
{
	delete m_color;
	delete m_add;
	delete m_mat;
}

size_t Sprite::Size() const
{
	size_t sz = 0;

	sz += sizeof(m_type);
	sz += sizeof(m_id);

	if (m_type == FID) {
		return sz;
	}
	
	if (m_color) {
		sz += sizeof(*m_color);
		sz += sizeof(*m_add);
	}
	if (m_mat) {
		sz += sizeof(*m_mat);
	}

	return sz;
}

void Sprite::Store(uint8_t** ptr) const
{
	pack2mem(m_type, ptr);
	pack2mem(m_id, ptr);

	if (m_type == FID) {
		return;
	}

	if (m_color) {
		pack2mem(m_color, ptr);
		pack2mem(m_add, ptr);
	}
	if (m_mat) {
		for (int i = 0; i < 6; ++i) {
			pack2mem(m_mat->m[i], ptr);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class Sprite::Matrix
//////////////////////////////////////////////////////////////////////////

Sprite::Matrix::
Matrix()
{
	m[0] = m[3] = 1024;
	m[1] = m[2] = m[4] = m[5] = 0;
}

//////////////////////////////////////////////////////////////////////////
// class Frame
//////////////////////////////////////////////////////////////////////////

Frame::~Frame()
{
	for_each(sprites.begin(), sprites.end(), DeletePointerFunctor<Sprite>());
}

size_t Frame::Size() const
{
	size_t sz = 0;
	sz += sizeof(uint16_t);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sz += sprites[i]->Size();
	}
	return sz;
}

void Frame::Store(uint8_t** ptr)
{
	uint16_t sz = sprites.size();
	pack2mem(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		sprites[i]->Store(ptr);
	}
}

//////////////////////////////////////////////////////////////////////////
// class Action
//////////////////////////////////////////////////////////////////////////

Action::~Action()
{
	for_each(frames.begin(), frames.end(), DeletePointerFunctor<Frame>());
}

size_t Action::Size() const
{
	size_t sz = 0;
	sz += action.Size();
	sz += sizeof(uint16_t);
	for (int i = 0, n = frames.size(); i < n; ++i) {
		sz += frames[i]->Size();
	}
	return sz;
}

void Action::Store(uint8_t** ptr)
{
	action.Store(ptr);

	uint16_t sz = frames.size();
	pack2mem(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		frames[i]->Store(ptr);
	}
}

//////////////////////////////////////////////////////////////////////////
// class Animation
//////////////////////////////////////////////////////////////////////////

Animation::Animation(lua_State* L, int id)
	: m_clipbox(NULL)
	, m_id(id)
	, m_type(ANIMATION)
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

	// clipbox
	lua_getfield(L, -1, "clipbox");
	if (lua_type(L, -1) != LUA_TNIL) {
		m_type = CLIPUI;
		m_clipbox = new Clipbox;
		int len = lua_rawlen(L, -1);
		assert(len == 4);
		for (int i = 1; i <= len; ++i) {
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			m_clipbox->cb[i-1] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
	}
	lua_pop(L, 1);

	// components
	lua_getfield(L, -1, "component");
	int len = lua_rawlen(L, -1);
	m_components.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		if (!LuaDataHelper::HasField(L, "id")) {
			if (LuaDataHelper::HasField(L, "font")) {
				m_components.push_back(new Label(L));
			} else if (LuaDataHelper::HasField(L, "name")) {
				m_components.push_back(new Mount(L));
			}
		} else if (LuaDataHelper::HasField(L, "name")) {
			m_components.push_back(new Switch(L));
		} else {
			m_components.push_back(new Component(L));
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	// actions
	len = lua_rawlen(L, -1);
	m_actions.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Action* action = new Action;

		lua_getfield(L, -1, "action");
		if (lua_type(L, -1) != LUA_TNIL) {
			std::string ac_str = lua_tostring(L, -1);
			action->action.SetString(ac_str);
		}
		lua_pop(L, 1);

		int len = lua_rawlen(L, -1);
		action->frames.reserve(len);
		for (int i = 1; i <= len; ++i)
		{
			Frame* frame = new Frame;

			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));
			int len = lua_rawlen(L, -1);
			frame->sprites.reserve(len);
			for (int i = 1; i <= len; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, -2);

	 			Sprite* spr = new Sprite(L, m_components.size());
	 			frame->sprites.push_back(spr);

				lua_pop(L, 1);
			}
			action->frames.push_back(frame);

			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		m_actions.push_back(action);
	}
}

Animation::~Animation()
{
	for_each(m_components.begin(), m_components.end(), DeletePointerFunctor<IPackNode>());
	for_each(m_actions.begin(), m_actions.end(), DeletePointerFunctor<Action>());
	delete m_clipbox;
}

size_t Animation::Size() const
{
	size_t sz = 0;

	sz += sizeof(m_type);
	sz += sizeof(m_id);
	if (m_clipbox) {
		sz += m_clipbox->Size();
	}
	sz += m_export_name.Size();

	sz += sizeof(uint16_t);
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		sz += m_components[i]->Size();
	}

	sz += sizeof(uint16_t);
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		sz += m_actions[i]->Size();
	}

	return sz;
}

void Animation::Store(uint8_t** ptr)
{
	pack2mem(m_type, ptr);
	pack2mem(m_id, ptr);

	if (m_clipbox) {
		for  (int i = 0; i < 4; ++i) {
			pack2mem(m_clipbox->cb[i], ptr);
		}
	}
	m_export_name.Store(ptr);

	int sz = m_components.size();
	memcpy(*ptr, &sz, sizeof(uint16_t));
	*ptr += sizeof(uint16_t);
	for (int i = 0; i < sz; ++i) {
		m_components[i]->Store(ptr);
	}

	sz = m_actions.size();
	memcpy(*ptr, &sz, sizeof(uint16_t));
	*ptr += sizeof(uint16_t);
	for (int i = 0; i < sz; ++i) {
		m_actions[i]->Store(ptr);
	}
}

}
}