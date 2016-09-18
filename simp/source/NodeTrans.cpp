#include "NodeTrans.h"
#include "simp_define.h"
#include "ImportStream.h"
#include "Allocator.h"

namespace simp
{

NodeTrans::NodeTrans()
	: name(NULL)
{
}

int NodeTrans::Size()
{
	return sizeof(NodeTrans) + PTR_SIZE_DIFF - sizeof(uint32_t);
}

NodeTrans* NodeTrans::LoadTrans(Allocator& alloc, ImportStream& is)
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