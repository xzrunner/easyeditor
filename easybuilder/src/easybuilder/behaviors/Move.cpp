
#include "Move.h"

#include "dataset/Actor.h"

using namespace ebuilder;

Move::Move(Actor* actor) 
	: Behavior(e_Move, actor)
	, speed(100) 
{
	start = end = actor->getPosition();
}
