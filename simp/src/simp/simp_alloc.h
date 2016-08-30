#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_alloc_h
#define simp_alloc_h

struct simp_allocator;

void* simp_alloc(struct simp_allocator*, int sz);

struct simp_allocator* simp_create_allocator();
void simp_release_allocator(struct simp_allocator*);

#endif // simp_alloc_h

#ifdef __cplusplus
}
#endif