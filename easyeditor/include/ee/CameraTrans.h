#ifndef _EASYEDITOR_CAMERA_TRANS_H_
#define _EASYEDITOR_CAMERA_TRANS_H_

namespace ee
{

class CameraTrans
{
public:
	CameraTrans() : enable_perspective(true) {}
	
public:
	bool enable_perspective;

}; // CameraTrans

}

#endif // _EASYEDITOR_CAMERA_TRANS_H_