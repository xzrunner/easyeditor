#include "sp_rnode.h"
#include "sp_region.h"
#include "sp_rtree_cfg.h"

#include <string.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#define MAX_NODES 1024
#define NODE_CAPACITY 32
#define FILL_FACTOR 0.4f

struct sp_rnode {
	void* ud;

	int level;

	struct sp_region region;

	struct sp_rnode* children[NODE_CAPACITY+1];
	int children_size;

	struct sp_rnode* parent;
};

// static struct sp_rnode FREELIST[MAX_NODES];
// static int NEXT_NODE = 0;

struct node_freelist {
	struct sp_rnode* list[MAX_NODES];
	int next;
};

static struct node_freelist* FREELIST;

void 
sp_node_prepare() {
	int sz = sizeof(struct node_freelist) + sizeof(struct sp_rnode) * MAX_NODES;
	FREELIST = (struct node_freelist*)malloc(sz);
	if (!FREELIST) {
		return;
	}
	memset(FREELIST, 0, sz);

	for (int i = 0; i < MAX_NODES; ++i) {
		FREELIST->list[i] = (struct sp_rnode*)((intptr_t)FREELIST + sizeof(struct node_freelist) + sizeof(struct sp_rnode) * i);
	}
}

struct sp_rnode* 
sp_rnode_create() {
	if (FREELIST->next < MAX_NODES) {
		struct sp_rnode* n = FREELIST->list[FREELIST->next++];
		memset(n, 0,sizeof(*n));
		REGION_INIT(n->region)
		return n;
	} else {
		return NULL;
	}
}

void 
sp_rnode_release(struct sp_rnode* n) {
	assert(FREELIST->next > 0);
	FREELIST->list[FREELIST->next--] = n;
}

static struct sp_rnode*
_find_least_enlargement(struct sp_rnode* node, struct sp_region* r) {
	assert(node->children_size > 0);
	struct sp_rnode* best = NULL;
	float area_min = FLT_MAX;
	struct sp_region comb;
	for (int i = 0; i < node->children_size; ++i) {
		struct sp_rnode* c = node->children[i];
		REGION_COMBINE(*r, c->region, comb)
			float area = REGION_AREA(c->region);
		float enlarge = REGION_AREA(comb) - area;
		if (enlarge < area_min) {
			area_min = enlarge;
			best = c;
		} else if (enlarge == area_min) {
			if (area < REGION_AREA(best->region)) {
				best = c;
			}
		}
	}
	return best;
}

struct sp_rnode* 
sp_rnode_choose_subtree(struct sp_rnode* node, struct sp_region* r) {
	if (node->children_size == 0) {
		return node;
	} 
	
	struct sp_rnode* child = NULL;
	switch (RTREE_STRATEGY) {
	case LINEAR:
	case QUADRATIC:
		child = _find_least_enlargement(node, r);
		break;
	}

	return sp_rnode_choose_subtree(child, r);
}

static void
_insert_data(struct sp_rnode* node, struct sp_region* r, void* ud) {
	assert(node->children_size < NODE_CAPACITY);

	struct sp_rnode* c = sp_rnode_create();
	c->ud = ud;
	c->region = *r;

	node->children[node->children_size++] = c;
	c->parent = node;

	struct sp_region comb;
	REGION_COMBINE(node->region, *r, comb)
	node->region = comb;
}

static void
_adjust_tree(struct sp_rnode* parent, struct sp_rnode* child, struct sp_region* ori_region) {
	bool contain = REGION_CONTAINS(parent->region, child->region);
	bool touch = REGION_TOUCH(parent->region, *ori_region);
	if (contain && !touch) {
		return;
	}

	if (!contain) {
		REGION_COMBINE(parent->region, child->region, parent->region);
	} else {
		REGION_INIT(parent->region);
		for (int i = 0; i < parent->children_size; ++i) {
			REGION_COMBINE(parent->region, parent->children[i]->region, parent->region);
		}
	}

	if (parent->parent) {
		_adjust_tree(parent->parent, parent, ori_region);
	}
}

static void
_adjust_tree2(struct sp_rnode* parent, struct sp_rnode* c1, struct sp_rnode* c2, struct sp_region* c1_ori_region) {
	bool contain = REGION_CONTAINS(parent->region, c1->region);
	bool touch = REGION_TOUCH(parent->region, *c1_ori_region);

	if (!contain) {
		REGION_COMBINE(parent->region, c1->region, parent->region);
	} else if (touch) {
		REGION_INIT(parent->region);
		for (int i = 0; i < parent->children_size; ++i) {
			REGION_COMBINE(parent->region, parent->children[i]->region, parent->region)
		}
	}

//	bool adjust = _insert_data(parent, &c2->region, c2->ud);
	// todo	
}

static void
_peek_seed(struct sp_rnode* node, int* seed1, int* seed2) {
	assert(node->children_size >= 2);
	if (node->children_size == 2) {
		*seed1 = 0;
		*seed2 = 1;
		return;
	}

	switch (RTREE_STRATEGY) {
	case LINEAR:
	case RSTAR:
		{
			float xmin_max = FLT_MIN, xmax_min = FLT_MAX;
			float ymin_max = FLT_MIN, ymax_min = FLT_MAX;
			int xmin_max_node, xmax_min_node, ymin_max_node, ymax_min_node;
			for (int i = 0; i < NODE_CAPACITY; ++i) {
				struct sp_rnode* n = node->children[i];
				if (n->region.xmin > xmin_max) xmin_max_node = i;
				if (n->region.xmax < xmax_min) xmax_min_node = i;
				if (n->region.ymin > ymin_max) ymin_max_node = i;
				if (n->region.ymax < ymax_min) ymax_min_node = i;
			}

			if (xmin_max - xmax_min > ymin_max - ymax_min) {
				*seed1 = xmin_max_node;
				*seed2 = xmax_min_node;
			} else {
				*seed1 = ymin_max_node;
				*seed2 = ymax_min_node;
			}
			if (*seed1 == *seed2) {
				*seed2 = (*seed2 == 0 ? 1 : *seed2 - 1);
			}
		}
		break;
	case QUADRATIC:
		{
			float max_area = FLT_MIN;
			for (int i = 0; i < NODE_CAPACITY; ++i) {
				for (int j = i + 1; j <= NODE_CAPACITY; ++j) {
					struct sp_rnode* node1 = node->children[i];
					struct sp_rnode* node2 = node->children[j];
					struct sp_region comb;
					REGION_COMBINE(node1->region, node2->region, comb);
					float area = REGION_AREA(comb) - REGION_AREA(node1->region) - REGION_AREA(node2->region);
					if (area > max_area) {
						max_area = area;
						*seed1 = i;
						*seed2 = j;
					}
				}
			}
		}
		break;
	default:
		assert(0);
	}
}

static void
_rtree_split(struct sp_rnode* node, struct sp_region* r, void* ud,
			 int* group1, int* group1_size,
			 int* group2, int* group2_size) {
	struct sp_rnode* new_node = sp_rnode_create();
	new_node->ud = ud;
	new_node->level = node->level + 1;
	new_node->region = *r;

	assert(node->children_size == NODE_CAPACITY);
	node->children[NODE_CAPACITY] = new_node;
	
	uint8_t mask[NODE_CAPACITY + 1];
	memset(mask, 0, NODE_CAPACITY + 1);

	int seed1, seed2;
	_peek_seed(node, &seed1, &seed2);
	mask[seed1] = mask[seed2] = 1;

	struct sp_region region1 = node->children[seed1]->region;
	struct sp_region region2 = node->children[seed2]->region;

	group1[(*group1_size)++] = seed1;
	group2[(*group2_size)++] = seed2;
	
	struct sp_region comb1, comb2;

	int min_load = floor(NODE_CAPACITY * FILL_FACTOR);
	int remain = NODE_CAPACITY + 1 - 2;
	while (remain > 0) {
		if (min_load - *group1_size == remain) {
			for (int i = 0; i < NODE_CAPACITY + 1; ++i) {
				if (mask[i]) {
					continue;
				}
				group1[(*group1_size)++] = i;
				mask[i] = 1;
				--remain;
			}			
		} else if (min_load - *group2_size == remain) {
			for (int i = 0; i < NODE_CAPACITY + 1; ++i) {
				if (mask[i]) {
					continue;
				}
				group2[(*group2_size)++] = i;
				mask[i] = 1;
				--remain;
			}
		} else {
			float area1 = REGION_AREA(region1);
			float area2 = REGION_AREA(region2);
			float m = FLT_MIN;
			float md1 = 0, md2 = 0;
			int sel;
			for (int i = 0; i < NODE_CAPACITY + 1; ++i) {
				if (mask[i]) {
					continue;
				}
				struct sp_rnode* c = node->children[i];
				REGION_COMBINE(c->region, region1, comb1);
				float d1 = REGION_AREA(comb1) - area1;
				REGION_COMBINE(c->region, region2, comb2);
				float d2 = REGION_AREA(comb2) - area2;
				float d = fabs(d1 - d2);
				if (d > m) {
					m = d;
					md1 = d1;
					md2 = d2;
					sel = i;
					if (RTREE_STRATEGY == LINEAR || RTREE_STRATEGY == RSTAR) {
						break;
					}
				}
			}
			bool is_group1 = false;
			if (md1 < md2) {
				group1[(*group1_size)++] = sel;
				is_group1 = true;
			} else if (md1 > md2) {
				group2[(*group2_size)++] = sel;
				is_group1 = false;
			} else if (area1 < area2) {
				group1[(*group1_size)++] = sel;
				is_group1 = true;
			} else if (area1 > area2) {
				group2[(*group2_size)++] = sel;
				is_group1 = false;
			} else if (group1_size < group2_size) {
				group1[(*group1_size)++] = sel;
				is_group1 = true;
			} else if (group1_size > group2_size) {
				group2[(*group2_size)++] = sel;
				is_group1 = false;
			} else {
				group1[(*group1_size)++] = sel;
				is_group1 = true;
			}
			mask[sel] = 1;
			--remain;
			if (is_group1) {
				REGION_COMBINE(node->children[sel]->region, region1, region1);
			} else {
				REGION_COMBINE(node->children[sel]->region, region2, region2);
			}
		}
	}
}

static struct sp_rnode*
_split(struct sp_rnode* node, struct sp_region* r, void* ud) {
	int group1[NODE_CAPACITY];
	int group2[NODE_CAPACITY];
	int group1_size = 0, group2_size = 0;
	switch (RTREE_STRATEGY) {
	case LINEAR:
	case QUADRATIC:
		_rtree_split(node, r, ud, group1, &group1_size, group2, &group2_size);
		break;
	case RSTAR:
		break;
	default:
		assert(0);
	}

	struct sp_rnode* new_node = sp_rnode_create();
	new_node->level = node->level;
	new_node->parent = node->parent;
	for (int i = 0; i < group2_size; ++i) {
		struct sp_rnode* c = node->children[group2[i]];
		new_node->children[new_node->children_size++] = c;
		REGION_COMBINE(c->region, new_node->region, new_node->region)
	}

	REGION_INIT(node->region)
	struct sp_rnode* children1[NODE_CAPACITY+1];
	int children1_size = 0;
	for (int i = 0; i < group1_size; ++i) {
		struct sp_rnode* c = node->children[group1[i]];
		children1[children1_size++] = c;
		REGION_COMBINE(c->region, node->region, node->region)
	}
	memcpy(node->children, children1, sizeof(node->children));
	node->children_size = children1_size;

	return new_node;
}

bool 
sp_rnode_insert_data(struct sp_rnode* node, struct sp_region* r, void* ud) {
	struct sp_region ori_region = node->region;
	if (node->children_size < NODE_CAPACITY) {
		bool adjusted = false;
		bool contain = REGION_CONTAINS(node->region, *r);
		_insert_data(node, r, ud);
		if (!contain && node->parent) {
			_adjust_tree(node->parent, node, &ori_region);
			adjusted = true;
		}
		return adjusted;
	} else {
		struct sp_rnode* new_node = _split(node, r, ud);
		if (node->parent) {
//			_adjust_tree(node->parent, );
		} else {
			
		}
	}
}

