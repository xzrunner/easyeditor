#include "AnimToBin.h"
#include "pack_unpack.h"
#include "typedef.h"

#include "spritepack.h"

namespace erespacker
{

int AnimToBin::Size(const PackAnimation* anim)
{
	int sz = 0;

	sz += sizeof(uint16_t);				// id
	sz += sizeof(uint8_t);				// type

	sz += sizeof(uint16_t);				// components size
	for (int i = 0, n = anim->components.size(); i < n; ++i) {
		sz += sizeof(uint16_t);								// id
		sz += sizeof_pack_str(anim->components[i].name);	// name
	}

	sz += sizeof(uint16_t);				// actions size
	for (int i = 0, n = anim->actions.size(); i < n; ++i) {
		sz += sizeof_pack_str(anim->actions[i].name);		// name
		sz += sizeof(uint16_t);								// sz
	}

	sz += sizeof(uint16_t);				// frames size
	for (int i = 0, n = anim->frames.size(); i < n; ++i) {
		sz += FrameSize(anim->frames[i]);
	}

	return sz;
}

void AnimToBin::Pack(const PackAnimation* anim, uint8_t** ptr)
{
	uint16_t id = anim->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_ANIMATION;
	pack(type, ptr);

	// components
	uint16_t sz = anim->components.size();
	pack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		uint16_t id = anim->components[i].node->GetSprID();
		pack(id, ptr);
		pack_str(anim->components[i].name, ptr);
	}

	// actions
	sz = anim->actions.size();
	pack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		pack_str(anim->actions[i].name, ptr);
		uint16_t sz = anim->actions[i].size;
		pack(sz, ptr);
	}

	// frames
	sz = anim->frames.size();
	pack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		PackFrame(anim->frames[i], ptr);
	}
}

int AnimToBin::FrameSize(const PackAnimation::Frame& frame)
{
	int ret = 0;
	ret += sizeof(uint16_t);		// parts size
	for (int i = 0, n = frame.parts.size(); i < n; ++i) {
		const PackAnimation::Part& part = frame.parts[i];

		ret += sizeof(uint8_t);		// type
		ret += sizeof(uint16_t);	// comp_idx

		const PackAnimation::SpriteTrans& t = part.t;
		if (!PackAnimation::IsMatrixIdentity(t.mat)) {
			ret += sizeof(int) * 6;
		}
		if (t.color != 0xffffffff) {
			ret += sizeof(uint32_t);
		}
		if (t.additive != 0) {
			ret += sizeof(uint32_t);
		}
		if ((t.rmap != 0xff0000ff && t.rmap != 0xff000000) || 
			(t.gmap != 0x00ff00ff && t.gmap != 0x00ff0000) || 
			(t.bmap != 0x0000ffff && t.bmap != 0x0000ff00)) {
			ret += sizeof(uint32_t) * 3;
		}
		if (t.blend != 0) {
			ret += sizeof(uint16_t);
		}
		if (t.filter != 0) {
			ret += sizeof(uint16_t);
		}
		if (t.camera != 0) {
			ret += sizeof(uint16_t);
		}
	}
	return ret;
}

void AnimToBin::PackFrame(const PackAnimation::Frame& frame, uint8_t** ptr)
{
	uint16_t sz = frame.parts.size();
	pack(sz, ptr);

	for (int i = 0; i < sz; ++i) 
	{
		const PackAnimation::Part& part = frame.parts[i];
		const PackAnimation::SpriteTrans& t = part.t;

		uint8_t type = TAG_ID;
		if (!PackAnimation::IsMatrixIdentity(t.mat)) {
			type |= TAG_MATRIX;
		}
		if (t.color != 0xffffffff) {
			type |= TAG_COLOR;
		}		
		if (t.additive != 0) {
			type |= TAG_ADDITIVE;
		}
		if ((t.rmap != 0xff0000ff && t.rmap != 0xff000000) || 
			(t.gmap != 0x00ff00ff && t.gmap != 0x00ff0000) || 
			(t.bmap != 0x0000ffff && t.bmap != 0x0000ff00)) {
			type |= TAG_COLMAP;
		}
		if (t.blend != 0) {
			type |= TAG_BLEND;
		}
		if (t.filter != 0) {
			type |= TAG_FILTER;
		}
		if (t.camera != 0) {
			type |= TAG_CAMERA;
		}

		pack(type, ptr);

		uint16_t comp_idx = part.comp_idx;
		pack(comp_idx, ptr);

		if (type & TAG_MATRIX) {
			for (int i = 0; i < 6; ++i) {
				int m = t.mat[i];
				pack(m, ptr);
			}
		}
		if (type & TAG_COLOR) {
			uint32_t col = t.color;
			pack(col, ptr);
		}
		if (type & TAG_ADDITIVE) {
			uint32_t add = t.additive;
			pack(add, ptr);
		}
		if (type & TAG_COLMAP) {
			uint32_t c = t.rmap;
			pack(c, ptr);
			c = t.gmap;
			pack(c, ptr);
			c = t.bmap;
			pack(c, ptr);
		}
		if (type & TAG_BLEND) {
			uint16_t blend = t.blend;
			pack(blend, ptr);
		}
		if (type & TAG_FILTER) {
			uint16_t filter = t.filter;
			pack(filter, ptr);
		}
		if (type & TAG_CAMERA) {
			uint16_t camera = t.camera;
			pack(camera, ptr);
		}
	}
}

}