#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_slot_h
#define rigging_slot_h

#include <stdint.h>

struct rg_slot {
	uint16_t joint;
	uint16_t skin;
};

#define SIZEOF_RG_SLOT (sizeof(struct rg_slot))

#endif // rigging_slot_h

#ifdef __cplusplus
}
#endif