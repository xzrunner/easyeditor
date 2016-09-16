#ifndef _SIMP_FROM_INT_H_
#define _SIMP_FROM_INT_H_

#include <sm_const.h>

namespace simp
{

inline
float int2time(int time) {
	return time * 0.001f;
}

inline
float int2radian(int deg) {
	return deg * SM_DEG_TO_RAD;
}

inline
float int2float100x(int i) {
	return i * 0.01f;
}

inline
float int2float1024x(int i) {
	return i / 1024.0f;
}

inline
bool int2bool(int i) {
	return i == 1 ? true : false;
}

}

#endif // _SIMP_FROM_INT_H_