#include "PackPicture.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

static const float SCALE = 16;

std::string PackPicture::ToString() const
{
	ebuilder::CodeGenerator gen;

	gen.line("{");
	gen.tab();

	lua::assign_with_end(gen, "type", "\"picture\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::IntToString(m_id));

	char buff[256];
	for (int i = 0, n = quads.size(); i < n; ++i) {
		const Quad& q = quads[i];

		std::string tex_str;

		int src[8];
		for (int j = 0; j < 4; ++j) {
			src[j*2] = (int)(q.texture_coord[j].x + 0.5f);
			src[j*2+1] = (int)(q.texture_coord[j].y + 0.5f);
		}
		sprintf(buff, "src = { %d, %d, %d, %d, %d, %d, %d, %d },", 
			src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7]);
		std::string src_str = buff;

		int screen[8];
		for (int j = 0; j < 4; ++j) {
			screen[j*2] = (int)(q.texture_coord[j].x * SCALE + 0.5f);
			screen[j*2+1] = -(int)(q.texture_coord[j].y * SCALE + 0.5f);
		}
		sprintf(buff, "screen = { %d, %d, %d, %d, %d, %d, %d, %d },", 
			screen[0], screen[1], screen[2], screen[3], screen[4], screen[5], screen[6], screen[7]);
		std::string screen_str = buff;

		lua::tableassign(gen, "", 3, tex_str.c_str(), src_str.c_str(), screen_str.c_str());
	}

	gen.line("},");
	gen.detab();

	return gen.toText();
}

}