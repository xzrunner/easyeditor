#ifndef _EASYEDITOR_COSINESMOOTH_H_
#define _EASYEDITOR_COSINESMOOTH_H_

#include "Vector.h"

#include <vector>

namespace ee
{

class CosineSmooth
{
public:
	static void Do(const std::vector<Vector>& src, float sampling_width,
		std::vector<Vector>& dst);

}; // CosineSmooth

}

#endif // _EASYEDITOR_COSINESMOOTH_H_
