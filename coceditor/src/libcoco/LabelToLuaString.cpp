#include "LabelToLuaString.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

void LabelToLuaString::Pack(const PackLabel* label, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + label->GetFilepath());

	lua::assign_with_end(gen, "type", "\"label\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(label->GetID()));

	std::ostringstream ss;
	ss << "font = \"" << label->font << "\", ";
	ss << "color = " << d2d::transColor(label->color, d2d::PT_BGRA) << ", ";
	ss << "align = " << (label->align_hori | (label->align_vert << 4)) << ", ";
	ss << "size = " << label->size << ", ";
	ss << "width = " << label->width << ", ";
	ss << "height = " << label->height << ", ";
	ss << "noedge = ";
	if (label->has_edge) {
		ss << "false";
	} else {
		ss << "true";
	}

	gen.line(ss.str());

	gen.detab();
	gen.line("},");
}

}