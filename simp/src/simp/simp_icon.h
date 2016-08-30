#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_pack_icon_h
#define simp_pack_icon_h

#include "simp_pack.h"

#include <stdint.h>

// todo
struct simp_icon {
	uint32_t type;
};

#define SIZEOF_ICON (sizeof(struct simp_icon))

void simp_import_icon(struct simp_import_stream*);

struct simp_icon_spr {
	uint32_t sym;
	uint16_t process;
	uint16_t dummy;
};

#define SIZEOF_ICON_SPR (sizeof(struct simp_icon_spr))

void simp_import_icon_spr(struct simp_import_stream*);

#endif // simp_pack_icon_h

#ifdef __cplusplus
}
#endif