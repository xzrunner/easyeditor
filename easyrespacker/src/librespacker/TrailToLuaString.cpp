#include "TrailToLuaString.h"

#include <easybuilder.h>

#include <mt_2d.h>

namespace lua = ebuilder::lua;

namespace erespacker
{

void TrailToLuaString::Pack(const PackTrail* trail, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + trail->GetFilepath());

	lua::assign_with_end(gen, "type", "\"trail\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(trail->GetSprID()));
	if (!trail->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + trail->export_name + "\"");
	}

	PackMT(trail, gen);

	gen.detab();
	gen.line("},");
}

void TrailToLuaString::PackMT(const PackTrail* trail, ebuilder::CodeGenerator& gen)
{
	lua::connect(gen, 2, 
		lua::assign("mode", trail->mode),
		lua::assign("count", trail->count));

	lua::connect(gen, 3, 
		lua::assign("life_begin", TransTime(trail->life_begin)),
		lua::assign("life_offset", TransTime(trail->life_offset)),
		lua::assign("fadeout_time", TransTime(trail->fadeout_time)));

	lua::TableAssign ta(gen, "component", true);
	if (trail->mode == T2D_MODE_IMAGE) {
		PackCompImages(trail->comp_images, gen);
	} else {
		assert(trail->mode == T2D_MODE_SHAPE);
		PackCompShapes(trail->comp_shapes, gen);
	}
}

void TrailToLuaString::PackCompImages(const std::vector<PackTrail::CompImage>& comp_images, ebuilder::CodeGenerator& gen)
{
	for (int i = 0, n = comp_images.size(); i < n; ++i) {
		const PackTrail::CompImage& comp = comp_images[i];

		lua::TableAssign ta(gen, "", true);

		lua::connect(gen, 1, 
			lua::assign("id", comp.node->GetSprID()));

		lua::connect(gen, 2, 
			lua::assign("scale_begin", comp.scale_begin),
			lua::assign("scale_end", comp.scale_end));

		lua::connect(gen, 4, 
			lua::assign("mul_col_begin", comp.mul_col_begin), 
			lua::assign("mul_col_end", comp.mul_col_end),
			lua::assign("add_col_begin", comp.add_col_begin), 
			lua::assign("add_col_end", comp.add_col_end));
	}
}

void TrailToLuaString::PackCompShapes(const std::vector<PackTrail::CompShape>& comp_shapes, ebuilder::CodeGenerator& gen)
{
	for (int i = 0, n = comp_shapes.size(); i < n; ++i) {
		const PackTrail::CompShape& comp = comp_shapes[i];

		lua::TableAssign ta(gen, "", true);

		lua::connect(gen, 2, 
			lua::assign("linewidth", comp.linewidth),
			lua::assign("acuity", comp.acuity));

		lua::connect(gen, 2, 
			lua::assign("col_begin", comp.col_begin),
			lua::assign("col_end", comp.col_end));
	}
}

}