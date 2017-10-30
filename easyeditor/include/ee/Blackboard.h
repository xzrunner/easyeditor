#ifndef _EASYEDITOR_BLACKBOARD_H_
#define _EASYEDITOR_BLACKBOARD_H_

#include <cu/cu_macro.h>

namespace ee
{

class Blackboard
{
public:
	bool visible_audio;
	
private:
	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}

#endif // _EASYEDITOR_BLACKBOARD_H_