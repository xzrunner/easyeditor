#ifndef _EASYEDITOR_COSINESMOOTH_H_
#define _EASYEDITOR_COSINESMOOTH_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class CosineSmooth
{
public:
	static void Do(const std::vector<sm::vec2>& src, float sampling_width,
		std::vector<sm::vec2>& dst);

}; // CosineSmooth

}

#endif // _EASYEDITOR_COSINESMOOTH_H_
