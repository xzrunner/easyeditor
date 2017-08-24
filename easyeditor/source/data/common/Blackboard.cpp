#include "Blackboard.h"

namespace ee
{

SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
	: visible_audio(true)
{
}

}