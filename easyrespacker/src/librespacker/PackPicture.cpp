#include "PackPicture.h"

#include "PictureToLuaString.h"
#include "PictureFromLua.h"

#include "PictureToBin.h"
#include "PictureFromBin.h"

#include <ee/TexturePacker.h>
#include <ee/Image.h>
#include <ee/Exception.h>
#include <ee/ImageData.h>

namespace erespacker
{

PackPicture::PackPicture(int id)
	: IPackNode(id)
{
}

void PackPicture::PackToLuaString(ebuilder::CodeGenerator& gen, 
								  const ee::TexturePacker& tp,
								  float scale) const
{
	PictureToLuaString::Pack(this, gen, tp, scale);
}

void PackPicture::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	PictureFromLua::Unpack(L, images, this);
}

int PackPicture::SizeOfPackToBin() const
{
	return PictureToBin::Size(this);
}

void PackPicture::PackToBin(uint8_t** ptr,
							const ee::TexturePacker& tp,
							float scale) const
{
	PictureToBin::Pack(this, ptr, tp, scale);
}

int PackPicture::SizeOfUnpackFromBin() const
{
	return PictureFromBin::Size(this);
}

void PackPicture::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	PictureFromBin::Unpack(ptr, images, this);
}

void PackPicture::GetImgSrcPos(const ee::TexturePacker& tp, const ee::Image* img, 
							   const sm::vec2* texture_coord, int* src)
{
	const ee::TexturePacker::Frame* tp_frame = tp.Query(img->GetFilepath());
	if (!tp_frame && ee::ImageDataMgr::Instance()->GetDefaultSym() != "") {
		tp_frame = tp.Query(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	if (!tp_frame) {
		std::string str = img->GetFilepath();
		throw ee::Exception("Image can't find in tp, %s", str.c_str());
	}
	if (!tp_frame->src.rotated) {
		int left   = static_cast<int>(tp_frame->dst.tex_coords[0].x), 
			bottom = static_cast<int>(tp_frame->dst.tex_coords[0].y);
		int width  = static_cast<int>(tp_frame->dst.tex_coords[3].x) - left,
			height = static_cast<int>(tp_frame->dst.tex_coords[1].y) - bottom;
		for (int i = 0; i < 4; ++i) {
			src[i*2]   = static_cast<int>(floor(left + width * texture_coord[i].x + 0.5f));
			src[i*2+1] = static_cast<int>(floor(bottom + height * texture_coord[i].y + 0.5f));
		}
	} else {
		int left   = static_cast<int>(tp_frame->dst.tex_coords[0].y), 
			bottom = static_cast<int>(tp_frame->dst.tex_coords[0].x);
		int width  = static_cast<int>(tp_frame->dst.tex_coords[3].y - left),
			height = static_cast<int>(tp_frame->dst.tex_coords[1].x - bottom);
		for (int i = 0; i < 4; ++i) {
			src[i*2]   = static_cast<int>(floor(bottom + height * texture_coord[i].y + 0.5f));
			src[i*2+1] = static_cast<int>(floor(left + width * texture_coord[i].x + 0.5f));
		}
	}

	int idx = tp.QueryIdx(img->GetFilepath());
	if (idx == -1) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	int h = tp.GetTextureHeight(idx);
	for (int i = 0; i < 4; ++i) {
		src[i*2+1] = h - src[i*2+1];
	}
}

}