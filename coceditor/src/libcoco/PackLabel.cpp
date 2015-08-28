#include "PackLabel.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

std::string PackLabel::ToString() const
{
	ebuilder::CodeGenerator gen;

	gen.line("{");
	gen.tab();

	lua::assign_with_end(gen, "type", "\"label\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::IntToString(m_id));

	std::ostringstream ss;
	ss << "font = \"" << font << "\", ";
	ss << "color = " << d2d::transColor(color, d2d::PT_BGRA) << ", ";
	ss << "align = " << (align_hori | (align_vert << 4)) << ", ";
	ss << "size = " << size << ", ";
	ss << "width = " << width << ", ";
	ss << "height = " << height << ", ";
	ss << "noedge = ";
	if (has_edge) {
		ss << "false";
	} else {
		ss << "true";
	}

	gen.line(ss.str());

	gen.line("},");
	gen.detab();

	return gen.toText();
}

}