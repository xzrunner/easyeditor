#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_spr_pack_h
#define simp_spr_pack_h

#define PTR_SIZE_DIFF (8 - sizeof(void *))
#define SIZEOF_POINTER 8

#define TYPE_EMPTY			0
#define TYPE_IMAGE			1
#define TYPE_SCALE9			3
#define TYPE_SCALE9_SPR		4
#define TYPE_LABEL			5

#define ANCHOR_ID 0xffffffff
#define SCREEN_SCALE 16

struct simp_pack {
	int min_id, max_id;
	void** sprs;

	struct simp_spr_pack* next;
};

#define SIZEOF_PACK (sizeof(struct sprite_pack) + 2 * PTR_SIZE_DIFF)

#endif // simp_spr_pack

#ifdef __cplusplus
}
#endif