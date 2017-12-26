#include "bsn/NodeSpr.h"

#include <bs/ImportStream.h>
#include <bs/typedef.h>
#include <bs/Serializer.h>

namespace bsn
{

NodeSpr::NodeSpr()
	: m_sym_path(nullptr)
	, m_name(nullptr)
	, m_type(0)
{
}

size_t NodeSpr::GetBinSize() const
{
	int sz = Size(m_type);
	sz += bs::pack_size(m_sym_path);
	sz += bs::pack_size(m_name);
	return sz;
}

void NodeSpr::StoreToBin(byte** data, size_t& length) const
{

}

void NodeSpr::StoreToJson(json::Value& val) const
{

}

NodeSpr* NodeSpr::Create(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	const char* sym_path = is.String(alloc);

	const char* name = is.String(alloc);

	uint32_t type = is.UInt32();

	size_t sz = Size(type);
	void* ptr = alloc.alloc<char>(ALIGN_4BYTE(sz));
	NodeSpr* spr = new (ptr) NodeSpr();
	spr->m_sym_path = sym_path;
	spr->m_name = name;
	spr->m_type = type;
	int idx = 0;
	if (type & SCALE_MASK) {
		spr->m_data[idx++] = is.UInt32();
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & SHEAR_MASK) {
		spr->m_data[idx++] = is.UInt32();
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & OFFSET_MASK) {
		spr->m_data[idx++] = is.UInt32();
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & POSITION_MASK) {
		spr->m_data[idx++] = is.UInt32();
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & ANGLE_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & COL_MUL_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & COL_ADD_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & COL_R_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & COL_G_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & COL_B_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & BLEND_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & FAST_BLEND_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & FILTER_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & DOWNSMAPLE_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}
	if (type & CAMERA_MASK) {
		spr->m_data[idx++] = is.UInt32();
	}

	return spr;
}

NodeSpr* NodeSpr::Create(mm::LinearAllocator& alloc, const json::Value& val)
{
	return nullptr;
}

size_t NodeSpr::Size(uint32_t type)
{
	size_t sz = sizeof(NodeSpr) + bs::PTR_SIZE_DIFF * 2 - sizeof(uint32_t);

	if (type & SCALE_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & SHEAR_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & OFFSET_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & POSITION_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & ANGLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & COL_MUL_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & COL_ADD_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & COL_R_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & COL_G_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & COL_B_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & FAST_BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & FILTER_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & DOWNSMAPLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & CAMERA_MASK) {
		sz += sizeof(uint32_t);
	}

	return sz;
}

}