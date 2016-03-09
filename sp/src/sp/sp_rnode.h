#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_partition_rnode_h
#define spatial_partition_rnode_h

#include <stdbool.h>

struct sp_rnode;
struct sp_region;

void sp_node_prepare();

struct sp_rnode* sp_rnode_create();
void sp_rnode_release(struct sp_rnode* n);

struct sp_rnode* sp_rnode_choose_subtree(struct sp_rnode*, struct sp_region*);

bool sp_rnode_insert_data(struct sp_rnode*, struct sp_region* r, void* ud);

#endif // spatial_partition_rnode_h

#ifdef __cplusplus
}
#endif