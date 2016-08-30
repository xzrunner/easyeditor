#ifndef _EASYSPRPACKER_TO_INT_H_
#define _EASYSPRPACKER_TO_INT_H_

#include <sm_const.h>

#include <math.h>

namespace esprpacker
{

inline
int time2int(float time) {
	return static_cast<int>(floor(time * 1000 + 0.5f));
}

inline
int radian2int(float r) {
	return static_cast<int>(floor(r * SM_RAD_TO_DEG + 0.5f));
}

inline
int float2int(float f) {
	return static_cast<int>(floor(f + 0.5f));
}

inline
int float100x2int(float f) {
	return float2int(f * 100);
}

inline
int float1024x2int(float f) {
	return float2int(f * 1024);
}

inline
int bool2int(bool b) {
	return b ? 1 : 0;
}

}

#endif // _EASYSPRPACKER_TO_INT_H_