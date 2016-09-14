#include "NodeComplex.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "Allocator.h"
#include "NodeTrans.h"

namespace simp
{

NodeComplex::NodeComplex(Allocator& alloc, ImportStream& is)
{
	// scissor
	for (int i = 0; i < 4; ++i) {
		uint16_t s = is.UInt16();
		scissor[i] = s;
	}

	// actions
	actions_n = is.UInt16();
	uint8_t* actions_ptr = static_cast<uint8_t*>(alloc.Alloc(ActionSize() * actions_n));
	for (int i = 0; i < actions_n; ++i) 
	{
		Action* dst = (Action*)(actions_ptr + ActionSize() * i);
		dst->name = is.String(alloc);
		dst->n = is.UInt16();
		dst->idx = (uint16_t*)(alloc.Alloc(sizeof(uint16_t) * dst->n));
		for (int j = 0; j < dst->n; ++j) {
			dst->idx[j] = is.UInt16();
		}
	}

	// sprs
	sprs_n = is.UInt16();
	sprs = (uint32_t*)(alloc.Alloc(sizeof(int32_t) * sprs_n));
	trans = static_cast<NodeTrans**>(alloc.Alloc(SIZEOF_POINTER * sprs_n));
	memset(trans, 0, SIZEOF_POINTER * sprs_n);
	for (int i = 0; i < sprs_n; ++i) {
		sprs[i] = is.UInt32();
		trans[i] = LoadTrans(alloc, is);
	}
}

int NodeComplex::Size()
{
	return sizeof(NodeComplex) + PTR_SIZE_DIFF * 2 - sizeof(Action);
}

int NodeComplex::ActionSize()
{
	return sizeof(Action) + PTR_SIZE_DIFF * 2;
}

NodeTrans* NodeComplex::LoadTrans(Allocator& alloc, ImportStream& is)
{
	uint32_t type = is.UInt32();
	
	int sz = NodeTrans::Size();
	if (type & NodeTrans::SCALE_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & NodeTrans::SHEAR_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & NodeTrans::OFFSET_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & NodeTrans::POSITION_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (type & NodeTrans::ANGLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::COL_MUL_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::COL_ADD_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::COL_R_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::COL_G_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::COL_B_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::FAST_BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::FILTER_MASK) {
		sz += sizeof(uint32_t);
	}
	if (type & NodeTrans::CAMERA_MASK) {
		sz += sizeof(uint32_t);
	}

	void* ptr = alloc.Alloc(ALIGN_4BYTE(sz));
	NodeTrans* trans = new (ptr) NodeTrans();
	trans->type = type;
	int idx = 0;
	if (type & NodeTrans::SCALE_MASK) {
		trans->data[idx++] = is.UInt32();
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::SHEAR_MASK) {
		trans->data[idx++] = is.UInt32();
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::OFFSET_MASK) {
		trans->data[idx++] = is.UInt32();
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::POSITION_MASK) {
		trans->data[idx++] = is.UInt32();
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::ANGLE_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::COL_MUL_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::COL_ADD_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::COL_R_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::COL_G_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::COL_B_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::BLEND_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::FAST_BLEND_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::FILTER_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	if (type & NodeTrans::CAMERA_MASK) {
		trans->data[idx++] = is.UInt32();
	}
	trans->name = is.String(alloc);

	return trans;
}

}