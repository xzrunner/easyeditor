#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

void PackAnimation::ToString(ebuilder::CodeGenerator& gen,
							 const TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::assign_with_end(gen, "type", "\"animation\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::IntToString(m_id));
	if (!export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + export_name + "\"");
	}

	// component
	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = components.size(); i < n; ++i) {
			const Component& comp = components[i];

			std::string id_str = lua::assign("id", d2d::StringTools::IntToString(comp.node->GetID()));
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
			FrameToString(frame, gen);
		}
	}

	gen.detab();
	gen.line("},");
}

void PackAnimation::CreateFramePart(const d2d::ISprite* spr, Frame& frame)
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	
	const IPackNode* node = factory->Create(spr);

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

void PackAnimation::FrameToString(const Frame& frame, ebuilder::CodeGenerator& gen)
{
	for (int i = 0, n = frame.parts.size(); i < n; ++i) 
	{
		const Part& part = frame.parts[i];
		const SpriteTrans& t = part.t;

		std::vector<std::string> params;

		// index
		std::string index_str = lua::assign("index", d2d::StringTools::IntToString(part.comp_idx));
		params.push_back(index_str);

		// mat
		std::string m[6];
		for (int i = 0; i < 6; ++i) {
			m[i] = wxString::FromDouble(t.mat[i]);
		}
		std::string mat_str = lua::tableassign("", 6, m[0].c_str(), m[1].c_str(), m[2].c_str(), 
			m[3].c_str(), m[4].c_str(), m[5].c_str());
		params.push_back(lua::assign("mat", mat_str));

		// color
		if (t.color != 0xffffffff) {
			params.push_back(d2d::StringTools::IntToString(t.color));
		}
		if (t.additive != 0) {
			params.push_back(d2d::StringTools::IntToString(t.additive));
		}
		if (t.rmap != 0xffff0000 || t.gmap != 0xff00ff00 || t.bmap != 0xff0000ff) {
			params.push_back(d2d::StringTools::IntToString(t.rmap));
			params.push_back(d2d::StringTools::IntToString(t.gmap));
			params.push_back(d2d::StringTools::IntToString(t.bmap));
		}

		lua::tableassign(gen, "", params);
	}
}

void PackAnimation::LoadSprTrans(const d2d::ISprite* spr, SpriteTrans& trans)
{
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
		trans.mat[i] = (int)(mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		trans.mat[i] = (int)(mat[i] * 16 + 0.5f);
	// flip y
	trans.mat[5] = -trans.mat[5];

	// color
	trans.color = d2d::trans_color2int(spr->multiCol, d2d::PT_BGRA);
	trans.additive = d2d::trans_color2int(spr->addCol, d2d::PT_BGRA);
	trans.rmap = d2d::trans_color2int(spr->r_trans, d2d::PT_BGRA);
	trans.gmap = d2d::trans_color2int(spr->g_trans, d2d::PT_BGRA);
	trans.bmap = d2d::trans_color2int(spr->b_trans, d2d::PT_BGRA);	
}

}