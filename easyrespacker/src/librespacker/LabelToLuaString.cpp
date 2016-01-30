#include "LabelToLuaString.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace librespacker
{

void LabelToLuaString::Pack(const PackLabel* label, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + label->GetFilepath());

	lua::assign_with_end(gen, "type", "\"label\"");
	lua::assign_with_end(gen, "id", d2d::StringHelper::ToString(label->GetSprID()));

	lua::connect(gen, 2, 
		lua::assign("width", label->width), 
		lua::assign("height", label->height));

	lua::connect(gen, 3, 
		lua::assign("font", label->font), 
		lua::assign("font_size", label->font_size),
		lua::assign("font_color", d2d::TransColor(label->font_color, d2d::PT_RGBA)));

	lua::connect(gen, 3, 
		lua::assign("edge", label->edge), 
		lua::assign("edge_size", label->edge_size),
		lua::assign("edge_color", d2d::TransColor(label->edge_color, d2d::PT_RGBA)));

	lua::connect(gen, 2, 
		lua::assign("align_hori", label->align_hori), 
		lua::assign("align_vert", label->align_vert));

	lua::connect(gen, 2, 
		lua::assign("space_hori", label->space_hori), 
		lua::assign("space_vert", label->space_vert));

	lua::connect(gen, 2, 
		lua::assign("text", "\"" + label->text + "\""),
		lua::assign("tid", "\"" + label->tid + "\""));

	gen.detab();
	gen.line("},");
}

}