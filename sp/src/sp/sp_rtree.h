#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_partition_rtree_h
#define spatial_partition_rtree_h

struct sp_rtree;

struct sp_rtree* sp_rtree_create();
void sp_rtree_release(struct sp_rtree*);

void sp_rtree_insert(struct sp_rtree*, struct sp_region*, void* ud);

#endif // spatial_partition_rtree_h

#ifdef __cplusplus
}
#endif