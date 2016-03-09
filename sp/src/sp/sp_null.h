#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_partition_null_h
#define spatial_partition_null_h

struct sp_null;

struct sp_null* sp_null_create();
void sp_null_release(struct sp_null*);

void sp_null_insert(struct sp_null*, struct sp_region*, void* ud);
bool sp_null_remove(struct sp_null*, void* ud);
void sp_null_clear(struct sp_null*);

void sp_null_query(struct sp_null*, struct sp_region*, void** ud, int* count);

void sp_null_debug_draw(struct sp_null*, void (*render)(float xmin, float ymin, float xmax, float ymax));

#endif // spatial_partition_null_h

#ifdef __cplusplus
}
#endif