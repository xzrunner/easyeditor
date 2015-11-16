#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_index_rtree_h
#define spatial_index_rtree_h

struct sidx_rtree;

struct sidx_rtree* sidx_rtree_create();
void sidx_rtree_release(struct sidx_rtree*);

void sidx_rtree_insert(struct sidx_rtree*, struct sidx_region*, void* ud);

#endif // spatial_index_rtree_h

#ifdef __cplusplus
}
#endif