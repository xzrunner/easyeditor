#include "epd_dataset.h"
#include "tools.h"
#include "LuaDataHelper.h"
#include "Exception.h"

#include <algorithm>
#include <assert.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

namespace epbin
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
// class String
//////////////////////////////////////////////////////////////////////////

String::String()
	: m_is_empty(true)
{
}

String::String(const std::string& str)
	: m_str(str)
	, m_is_empty(false)
{
}

void String::SetString(const std::string& str)
{
	m_str = str;
	m_is_empty = false;
}

size_t String::Size() const
{
	if (m_is_empty) {
		return sizeof(uint8_t);
	} else {
		assert(m_str.size() < 255);
		return sizeof(uint8_t) + m_str.size();
	}
}

void String::Store(uint8_t** ptr)
{
	if (m_is_empty) {
		uint8_t c = 255;
		memcpy(*ptr, &c, sizeof(uint8_t));
		*ptr += sizeof(uint8_t);
	} else {
		assert(m_str.size() < 255);
		uint8_t sz = m_str.size();
		memcpy(*ptr, &sz, sizeof(uint8_t));
		*ptr += sizeof(uint8_t);
		for (int i = 0; i < sz; ++i) {
			uint8_t c = m_str[i];
			memcpy(*ptr, &c, sizeof(uint8_t));
			*ptr += sizeof(uint8_t);
		}
	}
}

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

	memcpy(*ptr, &m_id, sizeof(m_id));
	*ptr += sizeof(m_id);

	uint16_t sz = m_parts.size();
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);
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
	memcpy(*ptr, &tex, sizeof(tex));
	*ptr += sizeof(tex);
	for (int i = 0; i < 8; ++i) {
		memcpy(*ptr, &src[i], sizeof(uint16_t));
		*ptr += sizeof(uint16_t);
	}
	for (int i = 0; i < 8; ++i) {
		memcpy(*ptr, &screen[i], sizeof(int32_t));
		*ptr += sizeof(int32_t);
	}
}

//////////////////////////////////////////////////////////////////////////
// class IComponent
//////////////////////////////////////////////////////////////////////////

size_t IComponent::Size() const
{
	return sizeof(m_type);
}

void IComponent::Store(uint8_t** ptr)
{
	memcpy(*ptr, &m_type, sizeof(m_type));
	*ptr += sizeof(m_type);
}

//////////////////////////////////////////////////////////////////////////
// class Component
//////////////////////////////////////////////////////////////////////////

Component::Component(lua_State* L)
	: IComponent(COMPONENT)
{
	m_id = LuaDataHelper::GetIntField(L, "id");
}

size_t Component::Size() const
{
	size_t sz = IComponent::Size();
	sz += sizeof(m_id);
	return sz;
}

void Component::Store(uint8_t** ptr)
{
	IComponent::Store(ptr);
	memcpy(*ptr, &m_id, sizeof(m_id));
	*ptr += sizeof(m_id);
}

//////////////////////////////////////////////////////////////////////////
// class Switch
//////////////////////////////////////////////////////////////////////////

Switch::Switch(lua_State* L)
	: IComponent(SWITCH)
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
	size_t sz = IComponent::Size();
	sz += sizeof(m_id);
	sz += m_name.Size();
	return sz;
}

void Switch::Store(uint8_t** ptr)
{
	IComponent::Store(ptr);
	memcpy(*ptr, &m_id, sizeof(m_id));
	*ptr += sizeof(m_id);
	m_name.Store(ptr);
}

//////////////////////////////////////////////////////////////////////////
// class Label
//////////////////////////////////////////////////////////////////////////

Label::Label(lua_State* L)
	: IComponent(LABEL)
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

	m_color = LuaDataHelper::GetDoubleField(L, "color");
	m_size = LuaDataHelper::GetIntField(L, "size");
	m_align = LuaDataHelper::GetIntField(L, "align");
	m_width = LuaDataHelper::GetIntField(L, "width");
	m_height = LuaDataHelper::GetIntField(L, "height");
}

size_t Label::Size() const
{
	size_t sz = IComponent::Size();
	sz += m_name.Size();
	sz += m_font.Size();
	sz += sizeof(m_color);
	sz += sizeof(m_size);
	sz += sizeof(m_align);
	sz += sizeof(m_width);
	sz += sizeof(m_height);
	return sz;
}

void Label::Store(uint8_t** ptr)
{
	IComponent::Store(ptr);

	m_name.Store(ptr);
	m_font.Store(ptr);

	memcpy(*ptr, &m_color, sizeof(m_color));
	*ptr += sizeof(m_color);

	memcpy(*ptr, &m_size, sizeof(m_size));
	*ptr += sizeof(m_size);

	memcpy(*ptr, &m_align, sizeof(m_align));
	*ptr += sizeof(m_align);

	memcpy(*ptr, &m_width, sizeof(m_width));
	*ptr += sizeof(m_width);

	memcpy(*ptr, &m_height, sizeof(m_height));
	*ptr += sizeof(m_height);
}

//////////////////////////////////////////////////////////////////////////
// class Mount
//////////////////////////////////////////////////////////////////////////

Mount::Mount(lua_State* L)
: IComponent(MOUNT)
{
	lua_getfield(L, -1, "name");
	if (lua_type(L, -1) != LUA_TNIL) {
		m_name.SetString(lua_tostring(L, -1));
	}
	lua_pop(L, 1);
}

size_t Mount::Size() const
{
	size_t sz = IComponent::Size();
	sz += m_name.Size();
	return sz;
}

void Mount::Store(uint8_t** ptr)
{
	IComponent::Store(ptr);
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

				m_mat->m[0] *= sx;
				m_mat->m[1] *= sy;
				m_mat->m[2] *= sx;
				m_mat->m[3] *= sy;
				m_mat->m[4] *= sx;
				m_mat->m[5] *= sy;

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
			*m_color = LuaDataHelper::GetDoubleField(L, "color");
			m_add = new uint32_t;
			*m_add = LuaDataHelper::GetDoubleField(L, "add");
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

void Sprite::Store(uint8_t** ptr)
{
	memcpy(*ptr, &m_type, sizeof(m_type));
	*ptr += sizeof(m_type);

	memcpy(*ptr, &m_id, sizeof(m_id));
	*ptr += sizeof(m_id);

	if (m_type == FID) {
		return;
	}

	if (m_color) {
		memcpy(*ptr, m_color, sizeof(*m_color));
		*ptr += sizeof(*m_color);

		memcpy(*ptr, m_add, sizeof(*m_add));
		*ptr += sizeof(*m_add);
	}
	if (m_mat) {
		for (int i = 0; i < 6; ++i) {
			memcpy(*ptr, &m_mat->m[i], sizeof(int32_t));
			*ptr += sizeof(int32_t);
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
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);

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
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);

	for (int i = 0; i < sz; ++i) {
		frames[i]->Store(ptr);
	}
}

//////////////////////////////////////////////////////////////////////////
// class Animation
//////////////////////////////////////////////////////////////////////////

std::set<std::string> Animation::m_set_export;

Animation::Animation(lua_State* L, int id)
	: m_clipbox(NULL)
	, m_id(id)
	, m_type(ANIMATION)
{
	// export
	lua_getfield(L, -1, "export");
	if (lua_type(L, -1) != LUA_TNIL) {
		std::string name = lua_tostring(L, -1);
		std::set<std::string>::iterator itr = m_set_export.find(name);
		if (itr != m_set_export.end()) {
			throw Exception("BinaryEPD::BinAni rename:%s\n", name);
		} else {
			m_set_export.insert(name);
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
	for_each(m_components.begin(), m_components.end(), DeletePointerFunctor<IComponent>());
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
	memcpy(*ptr, &m_type, sizeof(m_type));
	*ptr += sizeof(m_type);

	memcpy(*ptr, &m_id, sizeof(m_id));
	*ptr += sizeof(m_id);

	if (m_clipbox) {
		for  (int i = 0; i < 4; ++i) {
			memcpy(*ptr, &m_clipbox->cb[i], sizeof(int32_t));
			*ptr += sizeof(int32_t);
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