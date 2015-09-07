#include "AnimFromBin.h"

namespace libcoco
{

void AnimFromBin::Unpack(uint8_t** ptr, PackAnimation* anim)
{

}

void AnimFromBin::UnpackComponents(uint8_t** ptr, PackAnimation* anim)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t sz;
	unpack(sz, ptr);
	anim->components.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		PackAnimation::Component comp;

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
	}
}

void AnimFromBin::UnpackFrames(uint8_t** ptr, PackAnimation* anim)
{
	uint16_t sz;
	unpack(sz, ptr);
	anim->frames.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		Frame frame;
		UnpackFrame(frame, ptr);
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

		}
		if (type & TAG_COLOR) {

		}

		frame.parts.push_back(part);
	}
}

}