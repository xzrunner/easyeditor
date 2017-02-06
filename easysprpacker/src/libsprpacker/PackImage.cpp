#include "PackImage.h"
#include "binary_io.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageData.h>
#include <ee/TexturePacker.h>

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodePicture.h>
#include <simp/simp_types.h>

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
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	int idx = tp.QueryIdx(m_img->GetFilepath());
	if (idx == -1) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}

	char buff[256];

	sm::i16_rect quad;
	sm::ivec2 offset;
	GetPackRegion(tp, quad, offset);
	quad.xmin *= scale;
	quad.ymin *= scale;
	quad.xmax *= scale;
	quad.ymax *= scale;
	sprintf(buff, "{ %d, %d, %d, %d }",
		quad.xmin, quad.ymin, quad.xmax, quad.ymax);
	std::string src_str = buff;

	lua::connect(gen, 2, 
		lua::assign("tex", ee::StringHelper::ToString(idx)), 
		lua::assign("quad", src_str));

	if (offset.x != 0 || offset.y != 0) {
		lua::connect(gen, 2, 
			lua::assign("offx", offset.x), 
			lua::assign("offy", offset.y));
	}

	gen.detab();
	gen.line("},");
}

int PackImage::SizeOfUnpackFromBin() const
{
	return simp::NodePicture::Size();
}
	
int PackImage::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// texid
	sz += sizeof(uint16_t) * 4;		// texcoords
	sz += sizeof(int16_t) * 2;		// offset
	return sz;
}

void PackImage::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_IMAGE;
	pack(type, ptr);

	uint8_t idx = tp.QueryIdx(m_img->GetFilepath());
	if (idx == 0xff) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	pack(idx, ptr);

	sm::i16_rect quad;
	sm::ivec2 offset;
	GetPackRegion(tp, quad, offset);

	uint16_t xmin = quad.xmin;
	pack(xmin, ptr);
	uint16_t ymin = quad.ymin;
	pack(ymin, ptr);
	uint16_t xmax = quad.xmax;
	pack(xmax, ptr);
	uint16_t ymax = quad.ymax;
	pack(ymax, ptr);

	int16_t offx = offset.x;
	pack(offx, ptr);
	int16_t offy = offset.y;
	pack(offy, ptr);
}

void PackImage::Init(const ee::ImageSymbol* sym)
{
	m_img = sym->GetImage();
	if (m_img) {
		m_img->AddReference();
	}
}

void PackImage::GetPackRegion(const ee::TexturePacker& tp, sm::i16_rect& quad, sm::ivec2& offset) const
{
	const ee::TexturePacker::Frame* tp_frame = tp.Query(m_img->GetFilepath());
	if (!tp_frame && ee::ImageDataMgr::Instance()->GetDefaultSym() != "") {
		tp_frame = tp.Query(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	if (!tp_frame) {
		std::string str = m_img->GetFilepath();
		throw ee::Exception("Image can't find in tp, %s", str.c_str());
	}

	quad.xmin = static_cast<int>(tp_frame->dst.tex_coords[0].x);
	quad.ymin = static_cast<int>(tp_frame->dst.tex_coords[0].y);
	quad.xmax = static_cast<int>(tp_frame->dst.tex_coords[2].x);
	quad.ymax = static_cast<int>(tp_frame->dst.tex_coords[2].y);

	int idx = tp.QueryIdx(m_img->GetFilepath());
	if (idx == -1) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	int h = tp.GetTextureHeight(idx);
	quad.ymin = h - quad.ymin;
	quad.ymax = h - quad.ymax;

	offset.x = tp_frame->dst.offset.x * 2;
	offset.y = tp_frame->dst.offset.y * 2;
}

}