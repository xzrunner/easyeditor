#include "PackImage.h"
#include "binary_io.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageData.h>
#include <ee/TexturePacker.h>

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp_picture.h>
#include <simp_pack.h>

namespace esprpacker
{

PackImage::PackImage(const ee::ImageSymbol* sym)
	: m_img(NULL)
{
	Init(sym);
}

PackImage::~PackImage()
{
	if (m_img) {
		m_img->RemoveReference();
	}
}

void PackImage::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"image\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	int idx = tp.QueryIdx(m_img->GetFilepath());
	if (idx == -1) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	std::string tex_str = lua::assign("tex", ee::StringHelper::ToString(idx));

	char buff[256];

	int src[8];
	GetImgSrcPos(tp, m_texcoords, src);
	for (int i = 0; i < 8; ++i) {
		src[i] *= scale;
	}
	sprintf(buff, "src = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7]);
	std::string src_str = buff;

	lua::tableassign(gen, "", 2, tex_str, src_str);

	gen.detab();
	gen.line("},");
}

int PackImage::SizeOfUnpackFromBin() const
{
	return SIZEOF_PICTURE;
}
	
int PackImage::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// texid
	sz += sizeof(uint16_t) * 8;		// texcoords
	return sz;
}

void PackImage::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_IMAGE;
	pack(type, ptr);

	uint8_t idx = tp.QueryIdx(m_img->GetFilepath());
	if (idx == 0xff) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	pack(idx, ptr);

	int src[8];
	GetImgSrcPos(tp, m_texcoords, src);
	for (int i = 0; i < 8; ++i) {
		uint16_t p = static_cast<int>(src[i] * scale);
		pack(p, ptr);
	}
}

void PackImage::Init(const ee::ImageSymbol* sym)
{
	m_img = sym->GetImage();
	if (m_img) {
		m_img->AddReference();
	}

	m_texcoords[0].Set(0, 0);
	m_texcoords[1].Set(0, 1);
	m_texcoords[2].Set(1, 1);
	m_texcoords[3].Set(1, 0);
}

void PackImage::GetImgSrcPos(const ee::TexturePacker& tp, const sm::vec2* texture_coord, int* src) const
{
	const ee::TexturePacker::Frame* tp_frame = tp.Query(m_img->GetFilepath());
	if (!tp_frame && ee::ImageDataMgr::Instance()->GetDefaultSym() != "") {
		tp_frame = tp.Query(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	if (!tp_frame) {
		std::string str = m_img->GetFilepath();
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

	int idx = tp.QueryIdx(m_img->GetFilepath());
	if (idx == -1) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	int h = tp.GetTextureHeight(idx);
	for (int i = 0; i < 4; ++i) {
		src[i*2+1] = h - src[i*2+1];
	}
}

}