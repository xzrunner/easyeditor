#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_index_rnode_h
#define spatial_index_rnode_h

#include <stdbool.h>

struct sidx_rnode;
struct sidx_region;

struct sidx_rnode* sidx_rnode_new();

struct sidx_rnode* sidx_rnode_choose_subtree(struct sidx_rnode*, struct sidx_region*);

bool sidx_rnode_insert_data(struct sidx_rnode*, struct sidx_region* r, void* ud);

#endif // spatial_index_rnode_h

#ifdef __cplusplus
}
#endif