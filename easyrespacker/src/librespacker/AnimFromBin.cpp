#include "AnimFromBin.h"
#include "UnpackNodeFactory.h"
#include "pack_unpack.h"
#include "typedef.h"
#include "spritepack.h"

namespace librespacker
{

int AnimFromBin::Size(const PackAnimation* anim)
{
	int sz = SIZEOF_ANIMATION;

	// components
	sz += anim->components.size() * SIZEOF_COMPONENT;
	for (int i = 0, n = anim->components.size(); i < n; ++i) {
		sz += sizeof_unpack_str(anim->components[i].name);
	}

	// actions
	sz += anim->actions.size() * SIZEOF_ACTION;
	for (int i = 0, n = anim->actions.size(); i < n; ++i) {
		sz += sizeof_unpack_str(anim->actions[i].name);
	}

	// frames
	sz += anim->frames.size() * SIZEOF_FRAME;
	for (int i = 0, n = anim->frames.size(); i < n; ++i) {
		sz += FrameSize(anim->frames[i]);
	}

	return sz;
}

void AnimFromBin::Unpack(uint8_t** ptr, PackAnimation* anim)
{
	UnpackComponents(ptr, anim);
	UnpackActions(ptr, anim);
	UnpackFrames(ptr, anim);
}

int AnimFromBin::FrameSize(const PackAnimation::Frame& frame)
{
	size_t sz = 0;
	sz += frame.parts.size() * SIZEOF_PART;
	for (int i = 0, n = frame.parts.size(); i < n; ++i) {
		const PackAnimation::Part& part = frame.parts[i];
		if (!PackAnimation::IsMatrixIdentity(part.t.mat)) {
//			sz += SIZEOF_MATRIX;
			sz += sizeof(int) * 6;
		}
	}
	return sz;
}

void AnimFromBin::UnpackComponents(uint8_t** ptr, PackAnimation* anim)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t sz;
	unpack(sz, ptr);
	anim->components.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		anim->components.push_back(PackAnimation::Component());
		PackAnimation::Component& comp = anim->components[anim->components.size() - 1];

		uint16_t id;
		unpack(id, ptr);
		comp.node = factory->Query(id);
		if (!comp.node) {
			factory->AddUnassigned(id, &comp.node);
		}

		unpack_str(comp.name, ptr);
	}
}

void AnimFromBin::UnpackActions(uint8_t** ptr, PackAnimation* anim)
{
	uint16_t sz;
	unpack(sz, ptr);
	anim->actions.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		PackAnimation::Action action;

		unpack_str(action.name, ptr);

		uint16_t size;
		unpack(size, ptr);
		action.size = size;

		anim->actions.push_back(action);
	}
}

void AnimFromBin::UnpackFrames(uint8_t** ptr, PackAnimation* anim)
{
	uint16_t sz;
	unpack(sz, ptr);
	anim->frames.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		PackAnimation::Frame frame;
		UnpackFrame(ptr, frame);
		anim->frames.push_back(frame);
	}
}

void AnimFromBin::UnpackFrame(uint8_t** ptr, PackAnimation::Frame& frame)
{
	uint16_t sz;
	unpack(sz, ptr);
	frame.parts.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		PackAnimation::Part part;

		uint8_t type;
		unpack(type, ptr);

		uint16_t comp_idx;
		unpack(comp_idx, ptr);
		part.comp_idx = comp_idx;

		if (type & TAG_MATRIX) {
			for (int i = 0; i < 6; ++i) {
				int m;
				unpack(m, ptr);
				part.t.mat[i] = m;
			}
		}
		if (type & TAG_COLOR) {
			uint32_t col;
			unpack(col, ptr);
			part.t.color = col;
		}
		if (type & TAG_ADDITIVE) {
			uint32_t add;
			unpack(add, ptr);
			part.t.additive = add;
		}
		if (type & TAG_COLMAP) {
			uint32_t c;
			unpack(c, ptr);
			part.t.rmap = c;
			unpack(c, ptr);
			part.t.gmap = c;
			unpack(c, ptr);
			part.t.bmap = c;
		}

		frame.parts.push_back(part);
	}
}

}