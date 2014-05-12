
#include "Rotate.h"

#include "dataset/Actor.h"

using namespace ebuilder;

Rotate::Rotate(Actor* actor)
	: Behavior(e_Rotate, actor)
{
	start = end = 0;
	speed = 0.5f;
}
