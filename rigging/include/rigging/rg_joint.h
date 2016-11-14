#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_joint_h
#define rigging_joint_h

#include "rg_joint_pose.h"
#include "rg_utility.h"

#include <stdint.h>

struct rg_joint {
	const char* name;

	struct rg_joint_pose world_pose;
	struct rg_joint_pose local_pose;

	uint16_t skin;

	uint8_t  parent;

	uint8_t  children_count;
	uint8_t  children[1];
};

#define SIZEOF_RG_JOINT (sizeof(struct rg_joint) - sizeof(uint8_t) + PTR_SIZE_DIFF)

void rg_joint_update(struct rg_joint*, struct rg_skeleton*);

#endif // rigging_joint_h

#ifdef __cplusplus
}
#endif