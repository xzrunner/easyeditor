#include "AnimToLuaString.h"

#include <easybuilder.h>
#include <spritepack.h>

namespace lua = ebuilder::lua;

namespace librespacker
{

void AnimToLuaString::Pack(const PackAnimation* anim, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + anim->GetFilepath());

	lua::assign_with_end(gen, "type", "\"animation\"");
	lua::assign_with_end(gen, "id", d2d::StringHelper::ToString(anim->GetSprID()));
	if (!anim->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + anim->export_name + "\"");
	}

	// component
	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = anim->components.size(); i < n; ++i) {
			const PackAnimation::Component& comp = anim->components[i];

			int node_id = comp.node->GetSprID();
			if (node_id == ANCHOR_ID) {
				if (comp.name.empty()) {
					throw d2d::Exception("AnimToLuaString::Pack Anchor need a name.");
				}
				std::string name_str = lua::assign("name", "\""+comp.name+"\"");
				lua::tableassign(gen, "", 1, name_str);
			} else {
				std::string id_str = lua::assign("id", d2d::StringHelper::ToString(node_id));
				if (comp.name.empty()) {
					lua::tableassign(gen, "", 1, id_str);
				} else {
					std::string name_str = lua::assign("name", "\""+comp.name+"\"");
					lua::tableassign(gen, "", 2, id_str, name_str);
				}
			}
		}
	}

	// actions
	int s_frame = 0;
	for (int i = 0, n = anim->actions.size(); i < n; ++i)
	{
		const PackAnimation::Action& action = anim->actions[i];

		lua::TableAssign ta(gen, "", true);

		if (!action.name.empty()) {
			gen.line(lua::assign("action", "\"" + action.name + "\"") + ",");
		}

		// frames
		int e_frame = s_frame + action.size;
		for (int j = s_frame; j < e_frame; ++j) {
			const PackAnimation::Frame& frame = anim->frames[j];
			lua::TableAssign ta(gen, "", true);
			PackFrame(frame, gen);
		}
		s_frame = e_frame;
	}

	gen.detab();
	gen.line("},");
}

void AnimToLuaString::PackFrame(const PackAnimation::Frame& frame, ebuilder::CodeGenerator& gen)
{
	for (int i = 0, n = frame.parts.size(); i < n; ++i) 
	{
		const PackAnimation::Part& part = frame.parts[i];
		const PackAnimation::SpriteTrans& t = part.t;

		std::vector<std::string> params;

		// index
		std::string idx_str = d2d::StringHelper::ToString(part.comp_idx);
		std::string idx_assign = lua::assign("index", idx_str);
		params.push_back(idx_assign);

		// mat
		if (!PackAnimation::IsMatrixIdentity(t.mat)) {
			std::string m[6];
			for (int i = 0; i < 6; ++i) {
				m[i] = d2d::StringHelper::ToString(t.mat[i]);
			}
			std::string mat_str = lua::tableassign("", 6, m[0], m[1], m[2], m[3], m[4], m[5]);
			params.push_back(lua::assign("mat", mat_str));
		}

		// color
		if (t.color != 0xffffffff) {
			params.push_back(lua::assign("color", d2d::StringHelper::ToString(t.color)));
		}
		if (t.additive != 0) {
			params.push_back(lua::assign("add", d2d::StringHelper::ToString(t.additive)));
		}
		if (t.rmap != 0xff0000ff || t.gmap != 0x00ff00ff || t.bmap != 0x0000ffff) {
			params.push_back(lua::assign("rmap", d2d::StringHelper::ToString(t.rmap)));
			params.push_back(lua::assign("gmap", d2d::StringHelper::ToString(t.gmap)));
			params.push_back(lua::assign("bmap", d2d::StringHelper::ToString(t.bmap)));
		}

		if (params.size() > 1) {
			lua::tableassign(gen, "", params);
		} else {
			gen.line(idx_str + ",");
		}
	}
}

}