#pragma once

#include "predef.h"
#include N2_MAT_HEADER

#include <SM_Matrix.h>
#include <node0/typedef.h>

namespace n2
{

class DrawNode
{
public:
	static void Draw(const n0::SceneNodePtr& node, const N2_MAT& mt);

}; // DrawNode

}