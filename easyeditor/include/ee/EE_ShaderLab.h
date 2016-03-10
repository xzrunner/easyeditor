#ifndef _EASYEDITOR_SHADERLAB_H_
#define _EASYEDITOR_SHADERLAB_H_

#include "singleton.h"
#include "texture_format.h"

#include <stdint.h>

namespace ee
{

class ShaderLab
{
public:
	int CreateTexture(const uint8_t* data, int width, int height, EE_TEXTURE_FORMAT format);
	void ReleaseTexture(int id);
	void UpdateTexture(const uint8_t* data, int x, int y, int w, int h, int id);

	int GetTexGLID(int tex);
	
	void Flush();
	
	SINGLETON_DECLARATION(ShaderLab)

}; // ShaderLab

}

#endif // _EASYEDITOR_SHADERLAB_H_