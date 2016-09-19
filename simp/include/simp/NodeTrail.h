#ifndef _SIMP_NODE_TRAIL_H_
#define _SIMP_NODE_TRAIL_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeTrail : private cu::Uncopyable
{
public:
	struct Component
	{
		uint32_t col_begin, col_end;

		union {
			// image
			struct {
				uint32_t add_col_begin, add_col_end;
				uint32_t scale_begin, scale_end;
				uint32_t sym_id;
			} A;

			// shape
			struct {
				uint16_t size;
				uint16_t acuity;
			} B;
		} mode;
	};

	uint16_t count;

	uint16_t life_begin;
	uint16_t life_offset;

	uint16_t fadeout_time;

	uint8_t mode_type;
	uint8_t _pad[3];

	int n;
	Component components[1];

public:
	NodeTrail(Allocator& alloc, ImportStream& is);

	static int Size();
	static int ComponentSize();

}; // NodeTrail

}

#endif // _SIMP_NODE_TRAIL_H_