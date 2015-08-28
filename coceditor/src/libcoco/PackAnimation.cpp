#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

std::string PackAnimation::ToString() const
{
	ebuilder::CodeGenerator gen;

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
	for (int i = 0, n = actions.size(); i < n; ++i)
	{
		lua::TableAssign ta(gen, "", true);
		gen.line(lua::assign("action", "\"" + actions[i].name + "\"") + ",");
//		for ()
	}
	
	gen.line("},");
	gen.detab();

	return gen.toText();
}

void PackAnimation::CreateFramePart(const d2d::ISprite* spr, Frame& frame)
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	
	const IPackNode* node = factory->Create(spr);

	PackAnimation::Part part;
	part.comp_idx = AddComponent(node, spr->name);
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

void PackAnimation::LoadSprTrans(const d2d::ISprite* spr, SpriteTrans& trans)
{
	// | 1  ky    | | sx       | |  c  s    | | 1       |
	// | kx  1    | |    sy    | | -s  c    | |    1    |
	// |        1 | |        1 | |        1 | | x  y  1 |
	//     skew        scale        rotate        move

	trans.mat[1] = trans.mat[2] = trans.mat[4] = trans.mat[5] = 1;
	trans.mat[0] = trans.mat[3] = 1;

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
	trans.mat[0] = sx*c - ky*sy*s;
	trans.mat[1] = sx*s + ky*sy*c;
	trans.mat[2] = kx*sx*c - sy*s;
	trans.mat[3] = kx*sx*s + sy*c;
	trans.mat[4] = center.x/* * m_scale*/;
	trans.mat[5] = center.y/* * m_scale*/;

	for (size_t i = 0; i < 4; ++i)
		trans.mat[i] = (int)(trans.mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		trans.mat[i] = (int)(trans.mat[i] * 16 + 0.5f);
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