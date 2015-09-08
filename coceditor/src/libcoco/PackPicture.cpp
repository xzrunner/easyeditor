#include "PackPicture.h"
#include "spritepack.h"

#include "PictureToLuaString.h"
#include "PictureFromLua.h"

#include "PictureToBin.h"
#include "PictureFromBin.h"

namespace libcoco
{

PackPicture::PackPicture(int id)
	: IPackNode(id)
{
}

void PackPicture::PackToLuaString(ebuilder::CodeGenerator& gen, 
								  const d2d::TexturePacker& tp) const
{
	PictureToLuaString::Pack(this, gen, tp);
}

void PackPicture::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	PictureFromLua::Unpack(L, images, this);
}

int PackPicture::SizeOfPackToBin() const
{
	return PictureToBin::Size(this);
}

void PackPicture::PackToBin(uint8_t** ptr,
							const d2d::TexturePacker& tp) const
{
	PictureToBin::Pack(this, ptr, tp);
}

int PackPicture::SizeOfUnpackFromBin() const
{
	return PictureFromBin::Size(this);
}

void PackPicture::UnpackFromBin(uint8_t** ptr, const std::vector<d2d::Image*>& images)
{
	PictureFromBin::Unpack(ptr, images, this);
}

void PackPicture::GetImgSrcPos(const d2d::TexturePacker& tp, const d2d::Image* img, 
							   const d2d::Vector* texture_coord, int* src)
{
	const d2d::TexturePacker::Frame* tp_frame = tp.Query(img->GetFilepath());
	if (!tp_frame) {
		std::string str = img->GetFilepath();
		throw d2d::Exception("Image can't find in tp, %s", str.c_str());
	}
	if (!tp_frame->src.rotated) {
		int left = tp_frame->dst.tex_coords[0].x, bottom = tp_frame->dst.tex_coords[0].y;
		int width = tp_frame->dst.tex_coords[3].x - left,
			height = tp_frame->dst.tex_coords[1].y - bottom;
		for (int i = 0; i < 4; ++i) {
			src[i*2] = floor(left + width * texture_coord[i].x + 0.5f);
			src[i*2+1] = floor(bottom + height * texture_coord[i].y + 0.5f);
		}
	} else {
		int left = tp_frame->dst.tex_coords[0].y, bottom = tp_frame->dst.tex_coords[0].x;
		int width = tp_frame->dst.tex_coords[3].y - left,
			height = tp_frame->dst.tex_coords[1].x - bottom;
		for (int i = 0; i < 4; ++i) {
			src[i*2] = floor(bottom + height * texture_coord[i].y + 0.5f);
			src[i*2+1] = floor(left + width * texture_coord[i].x + 0.5f);
		}
	}

	int h = tp.GetTextureHeight(tp.QueryIdx(img->GetFilepath()));
	for (int i = 0; i < 4; ++i) {
		src[i*2+1] = h - src[i*2+1];
	}
}

}