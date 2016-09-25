#ifndef _EASYSKELETON_SKELETON_H_
#define _EASYSKELETON_SKELETON_H_

#include <sprite2/Skeleton.h>

#include <SM_Vector.h>

namespace eskeleton
{

class Joint;

class Skeleton : public s2::Skeleton
{
public:
	
	Joint* QueryByPos(const sm::vec2& pos);

private:
	

}; // Skeleton

}

#endif // _EASYSKELETON_SKELETON_H_