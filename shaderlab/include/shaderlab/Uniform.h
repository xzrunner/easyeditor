#ifndef _SHADERLAB_UNIFORM_H_
#define _SHADERLAB_UNIFORM_H_

#include <SM_Matrix.h>

namespace sl
{

struct UniformMat4
{
	int id;
	sm::mat4 mat;

	UniformMat4() : id(0) {}
};

}

#endif // _SHADERLAB_UNIFORM_H_