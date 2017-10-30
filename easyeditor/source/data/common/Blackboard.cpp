#include "Blackboard.h"

namespace ee
{

CU_SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
	: visible_audio(true)
{
}

}