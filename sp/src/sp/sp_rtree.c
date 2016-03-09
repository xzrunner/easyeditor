#include "sp_rtree.h"
#include "sp_rnode.h"
#include "sp_rtree_cfg.h"

#include <stdlib.h>
#include <string.h>

struct sp_rtree {
	struct sp_rnode* root;
};

struct sp_rtree* 
sp_rtree_create() {
	int sz = sizeof(struct sp_rtree);
	struct sp_rtree* tree = (struct sp_rtree*)malloc(sz);
	memset(tree, 0, sz);

	tree->root = sp_rnode_new();

	return tree;
}

void 
sp_rtree_release(struct sp_rtree* tree) {
	free(tree);
}

void 
sp_rtree_insert(struct sp_rtree* tree, struct sp_region* r, void* ud) {
	struct sp_rnode* n = sp_rnode_choose_subtree(tree->root, r);
}