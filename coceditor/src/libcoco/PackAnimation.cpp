#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "UnpackNodeFactory.h"

#include <easybuilder.h>
#include <epbin.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

PackAnimation::PackAnimation(int id)
	: IPackNode(id)
{
}

void PackAnimation::PackToLuaString(ebuilder::CodeGenerator& gen, const d2d::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + m_filepath);

	lua::assign_with_end(gen, "type", "\"animation\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(m_id));
	if (!export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + export_name + "\"");
	}

	// component
	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = components.size(); i < n; ++i) {
			const Component& comp = components[i];

			std::string id_str = lua::assign("id", d2d::StringTools::ToString(comp.node->GetID()));
			if (comp.name.empty()) {
				lua::tableassign(gen, "", 1, id_str.c_str());
			} else {
				std::string name_str = lua::assign("name", "\""+comp.name+"\"");
				lua::tableassign(gen, "", 2, id_str.c_str(), name_str.c_str());
			}
		}
	}

	// actions
	int s_frame = 0;
	for (int i = 0, n = actions.size(); i < n; ++i)
	{
		const Action& action = actions[i];

		lua::TableAssign ta(gen, "", true);

		if (!action.name.empty()) {
			gen.line(lua::assign("action", "\"" + action.name + "\"") + ",");
		}

		// frames
		int e_frame = s_frame + action.size;
		for (int j = s_frame; j < e_frame; ++j) {
			const Frame& frame = frames[j];
			lua::TableAssign ta(gen, "", true);
			PackFrameToLuaString(frame, gen);
		}
	}

	gen.detab();
	gen.line("},");
}

void PackAnimation::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	Clear();

	if (epbin::LuaDataHelper::HasField(L, "export")) {
		export_name = epbin::LuaDataHelper::GetStringField(L, "export");
	}

	UnpackComponentsFromLua(L);
	UnpackFramesFromLua(L);
}

void PackAnimation::CreateFramePart(const d2d::ISprite* spr, Frame& frame)
{	
	const IPackNode* node = PackNodeFactory::Instance()->Create(spr);

	PackAnimation::Part part;
	std::string name = "";
	if (!spr->name.empty() && spr->name[0] != '_') {
		name = spr->name;
	}
	part.comp_idx = AddComponent(node, name);
	PackAnimation::LoadSprTrans(spr, part.t);

	frame.parts.push_back(part);
}

int PackAnimation::AddComponent(const IPackNode* node, const std::string& name)
{
	for (int i = 0, n = components.size(); i < n; ++i) {
		if (components[i].node == node && components[i].name == name) {
			return i;
		}
	}

	Component comp;
	comp.node = node;
	comp.name = name;
	components.push_back(comp);
	return components.size() - 1;
}

void PackAnimation::Clear()
{
	export_name.clear();
	components.clear();
	actions.clear();
	frames.clear();
}

void PackAnimation::UnpackComponentsFromLua(lua_State* L)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	lua_getfield(L, -1, "component");
	int comp_sz = lua_rawlen(L, -1);
	components.reserve(comp_sz);
	for (int i = 1; i <= comp_sz; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		components.push_back(Component());
		Component& comp = components[components.size() - 1];
		int id = epbin::LuaDataHelper::GetIntField(L, "id");
		comp.node = factory->Query(id);
		if (!comp.node) {
			factory->AddUnassigned(id, &comp.node);
		}
		if (epbin::LuaDataHelper::HasField(L, "name")) {
			comp.name = epbin::LuaDataHelper::GetStringField(L, "name");
		}

		lua_pop(L, 1);
	}
	lua_pop(L, 1);	
}

void PackAnimation::UnpackFramesFromLua(lua_State* L)
{
	int action_sz = lua_rawlen(L, -1);
	for (int i = 1; i <= action_sz; ++i)
	{
		Action action;

		if (epbin::LuaDataHelper::HasField(L, "name")) {
			action.name = epbin::LuaDataHelper::GetStringField(L, "action");
		}

		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));
		int frame_sz = lua_rawlen(L, -1);
		action.size = frame_sz;
		for (int j = 1; j <= frame_sz; ++j) {
			lua_pushinteger(L, j);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));

			Frame frame;
			UppackFrameFromLua(L, frame);
			frames.push_back(frame);

			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		actions.push_back(action);
	}
}

void PackAnimation::UppackFrameFromLua(lua_State* L, Frame& frame)
{
	int spr_sz = lua_rawlen(L, -1);
	for (int i = 1; i <= spr_sz; ++i) 
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);

		Part part;
		UppackPartFromLua(L, part);
		frame.parts.push_back(part);

		lua_pop(L, 1);
	}
}

void PackAnimation::UppackPartFromLua(lua_State* L, Part& part)
{
	if (lua_isnumber(L, -1)) {
		part.comp_idx = lua_tointeger(L, -1);
	} else if (lua_istable(L, -1)) {
		part.comp_idx = epbin::LuaDataHelper::GetIntField(L, "index");
		if (epbin::LuaDataHelper::HasField(L, "mat")) {
			lua_getfield(L, -1, "mat");
			assert(lua_type(L, -1) != LUA_TNIL);
			int len = lua_rawlen(L, -1);
			assert(len == 6);
			for (int i = 1; i <= len; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				part.t.mat[i - 1] = lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}
		if (epbin::LuaDataHelper::HasField(L, "color")) {
			part.t.color = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "color");
		}
		if (epbin::LuaDataHelper::HasField(L, "add")) {
			part.t.additive = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "add");
		}
		if (epbin::LuaDataHelper::HasField(L, "rmap")) {
			part.t.rmap = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "rmap");
		}
		if (epbin::LuaDataHelper::HasField(L, "gmap")) {
			part.t.gmap = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "gmap");
		}
		if (epbin::LuaDataHelper::HasField(L, "bmap")) {
			part.t.bmap = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "bmap");
		}
	} else {
		throw d2d::Exception("PackAnimation::UnpackFromLua unknown item type.");
	}
}

void PackAnimation::PackFrameToLuaString(const Frame& frame, ebuilder::CodeGenerator& gen)
{
	for (int i = 0, n = frame.parts.size(); i < n; ++i) 
	{
		const Part& part = frame.parts[i];
		const SpriteTrans& t = part.t;

		std::vector<std::string> params;

		// index
		std::string idx_str = d2d::StringTools::ToString(part.comp_idx);
		std::string idx_assign = lua::assign("index", idx_str);
		params.push_back(idx_assign);

		// mat
		if (!IsMatrixIdentity(t.mat)) {
			std::string m[6];
			for (int i = 0; i < 6; ++i) {
				m[i] = d2d::StringTools::ToString(t.mat[i]);
			}
			std::string mat_str = lua::tableassign("", 6, m[0].c_str(), m[1].c_str(), m[2].c_str(), 
				m[3].c_str(), m[4].c_str(), m[5].c_str());
			params.push_back(lua::assign("mat", mat_str));
		}

		// color
		if (t.color != 0xffffffff) {
			params.push_back(lua::assign("color", d2d::StringTools::ToString(t.color)));
		}
		if (t.additive != 0) {
			params.push_back(lua::assign("add", d2d::StringTools::ToString(t.additive)));
		}
		if (t.rmap != 0xffff0000 || t.gmap != 0xff00ff00 || t.bmap != 0xff0000ff) {
			params.push_back(lua::assign("rmap", d2d::StringTools::ToString(t.rmap)));
			params.push_back(lua::assign("gmap", d2d::StringTools::ToString(t.gmap)));
			params.push_back(lua::assign("bmap", d2d::StringTools::ToString(t.bmap)));
		}

		if (params.size() > 1) {
			lua::tableassign(gen, "", params);
		} else {
			gen.line(idx_str + ",");
		}
	}
}

void PackAnimation::LoadSprTrans(const d2d::ISprite* spr, SpriteTrans& trans)
{
	LoadSprMat(spr, trans);
	LoadSprColor(spr, trans);
}

void PackAnimation::LoadSprMat(const d2d::ISprite* spr, SpriteTrans& trans)
{
	if (dynamic_cast<const d2d::ImageSprite*>(spr)) {
		return;
	}

	float mat[6];

	// | 1  ky    | | sx       | |  c  s    | | 1       |
	// | kx  1    | |    sy    | | -s  c    | |    1    |
	// |        1 | |        1 | |        1 | | x  y  1 |
	//     skew        scale        rotate        move

	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	d2d::Vector center = spr->GetCenter();

	bool xmir, ymir;
	spr->GetMirror(xmir, ymir);

	float sx = spr->GetScale().x,
		sy = spr->GetScale().y;
	if (xmir) {
		sx = -sx;
	}
	if (ymir) {
		sy = -sy;
	}

	float c = cos(-spr->GetAngle()),
		s = sin(-spr->GetAngle());
	float kx = -spr->GetShear().x,
		ky = -spr->GetShear().y;
	mat[0] = sx*c - ky*sy*s;
	mat[1] = sx*s + ky*sy*c;
	mat[2] = kx*sx*c - sy*s;
	mat[3] = kx*sx*s + sy*c;
	mat[4] = center.x/* * m_scale*/;
	mat[5] = center.y/* * m_scale*/;

	for (size_t i = 0; i < 4; ++i)
		trans.mat[i] = floor(mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		trans.mat[i] = floor(mat[i] * 16 + 0.5f);
	// flip y
	trans.mat[5] = -trans.mat[5];
}

void PackAnimation::LoadSprColor(const d2d::ISprite* spr, SpriteTrans& trans)
{
	trans.color = d2d::trans_color2int(spr->multiCol, d2d::PT_BGRA);
	trans.additive = d2d::trans_color2int(spr->addCol, d2d::PT_BGRA);
	trans.rmap = d2d::trans_color2int(spr->r_trans, d2d::PT_BGRA);
	trans.gmap = d2d::trans_color2int(spr->g_trans, d2d::PT_BGRA);
	trans.bmap = d2d::trans_color2int(spr->b_trans, d2d::PT_BGRA);	
}

bool PackAnimation::IsMatrixIdentity(const int* mat)
{
	return mat[0] == 1024 && mat[3] == 1024 
		&& mat[1] == 0 && mat[2] == 0 && mat[4] == 0 && mat[5] == 0;
}

}