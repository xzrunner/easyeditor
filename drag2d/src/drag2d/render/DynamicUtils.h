#ifndef _DRAG2D_DYNAMIC_UTILS_H_
#define _DRAG2D_DYNAMIC_UTILS_H_

namespace d2d
{

typedef unsigned int GLuint;

class TPNode;

class DynamicUtils
{
public:
	static GLuint InitTexture(int width, int height, int tex_id = 0);

	static GLuint InitFBO(int tex_id, int fbo_id = 0);

	static TPNode* InitRoot(int width, int height);

}; // DynamicUtils

}

#endif // _DRAG2D_DYNAMIC_UTILS_H_