#include "sidx_rtree.h"
#include "sidx_rnode.h"
#include "sidx_rtree_cfg.h"

#include <stdlib.h>
#include <string.h>

struct sidx_rtree {
	struct sidx_rnode* root;
};

struct sidx_rtree* 
sidx_rtree_create() {
	int sz = sizeof(struct sidx_rtree);
	struct sidx_rtree* tree = (struct sidx_rtree*)malloc(sz);
	memset(tree, 0, sz);

	tree->root = sidx_rnode_new();

	return tree;
}

void 
sidx_rtree_release(struct sidx_rtree* tree) {
	free(tree);
}

void 
sidx_rtree_insert(struct sidx_rtree* tree, struct sidx_region* r, void* ud) {
	struct sidx_rnode* n = sidx_rnode_choose_subtree(tree->root, r);
}