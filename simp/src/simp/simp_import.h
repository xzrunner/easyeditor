#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_import_h
#define simp_import_h

#include <stdint.h>

struct simp_import_stream;

uint8_t simp_import_uint8(struct simp_import_stream*);
uint16_t simp_import_uint16(struct simp_import_stream*);
uint32_t simp_import_uint32(struct simp_import_stream*);
const char* simp_import_string(struct simp_import_stream*);

uint32_t simp_import_rgba(struct simp_import_stream*);
uint32_t simp_import_abgr(struct simp_import_stream*);

void* simp_import_alloc(struct simp_import_stream*, int sz);

struct simp_spr_pack;

struct simp_spr_pack* simp_import_pkg(void* data, int sz);

#endif // simp_import_h

#ifdef __cplusplus
}
#endif