
#include "MouseButton.h"

using namespace ebuilder;

MouseButton::MouseButton(Actor* actor) 
	: Behavior(e_MouseButton, actor)
	, bPress(true) 
{}

MouseButton::~MouseButton() 
{
	for_each(satisfies.begin(), satisfies.end(), DeletePointerFunctor<Behavior>());
	for_each(notSatisfies.begin(), notSatisfies.end(), DeletePointerFunctor<Behavior>());
}
