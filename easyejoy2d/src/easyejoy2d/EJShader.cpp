#include "EJShader.h"

#include <shader.h>

namespace eejoy2d
{

void EJShader::Reset()
{
	shader_texture(-1);
	shader_program(-1);
}

}