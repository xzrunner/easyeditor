#ifndef _EASYEDITOR_SHADERLAB_H_
#define _EASYEDITOR_SHADERLAB_H_

#include "singleton.h"

#include <stdint.h>

namespace ee
{

class ShaderLab
{
public:
	void Init();

	int CreateTexture(const uint8_t* data, int width, int height, int format);
	void ReleaseTexture(int id);
	
	SINGLETON_DECLARATION(ShaderLab)

}; // ShaderLab

}

#endif // _EASYEDITOR_SHADERLAB_H_