#ifndef _EASYEDITOR_BLACKBOARD_H_
#define _EASYEDITOR_BLACKBOARD_H_

#include <CU_Singleton.h>

namespace ee
{

class Blackboard
{
public:
	bool visible_audio;
	
private:
	SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}

#endif // _EASYEDITOR_BLACKBOARD_H_