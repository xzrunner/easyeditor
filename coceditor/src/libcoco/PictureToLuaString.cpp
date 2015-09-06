#include "PictureToLuaString.h"
#include "tools.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

void PictureToLuaString::Pack(const PackPicture* pic, 
							  ebuilder::CodeGenerator& gen, 
							  const d2d::TexturePacker& tp)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + pic->GetFilepath());

	lua::assign_with_end(gen, "type", "\"picture\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(pic->GetID()));

	for (int i = 0, n = pic->quads.size(); i < n; ++i) {
		PackQuad(pic->quads[i], gen, tp);
	}

	gen.detab();
	gen.line("},");
}

void PictureToLuaString::PackQuad(const PackPicture::Quad& quad, 
								  ebuilder::CodeGenerator& gen, 
								  const d2d::TexturePacker& tp)
{
	int idx = tp.QueryIdx(quad.img->GetFilepath());
	std::string tex_str = lua::assign("tex", d2d::StringTools::ToString(idx));

	char buff[256];

	int src[8];
	PackPicture::GetImgSrcPos(tp, quad.img, quad.texture_coord, src);
	sprintf(buff, "src = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7]);
	std::string src_str = buff;

	int screen[8];
	for (int j = 0; j < 4; ++j) {
		screen[j*2] = floor(quad.screen_coord[j].x * SCALE + 0.5f);
		screen[j*2+1] = -floor(quad.screen_coord[j].y * SCALE + 0.5f);
	}
	sprintf(buff, "screen = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		screen[0], screen[1], screen[2], screen[3], screen[4], screen[5], screen[6], screen[7]);
	std::string screen_str = buff;

	lua::tableassign(gen, "", 3, tex_str.c_str(), src_str.c_str(), screen_str.c_str());
}

}