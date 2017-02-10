#include "SkinVertex.h"

#include <string.h>

namespace pm
{

SkinVertex::SkinVertex()
{
	memset(joint_id, 0xff, sizeof(joint_id));
	memset(joint_weight, 0, sizeof(joint_weight));
}

}