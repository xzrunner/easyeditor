#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_index_null_h
#define spatial_index_null_h

struct sidx_null;

struct sidx_null* sidx_null_create();
void sidx_null_release(struct sidx_null*);

void sidx_null_insert(struct sidx_null*, struct sidx_region*, void* ud);
bool sidx_null_remove(struct sidx_null*, void* ud);
void sidx_null_clear(struct sidx_null*);

void sidx_null_query(struct sidx_null*, struct sidx_region*, void** ud, int* count);

void sidx_null_debug_draw(struct sidx_null*, void (*render)(float xmin, float ymin, float xmax, float ymax));

#endif // spatial_index_null_h

#ifdef __cplusplus
}
#endif