#include "EJShader.h"

#include <dtex_shader.h>

namespace eejoy2d
{

void EJShader::Reset()
{
	dtex_shader_texture(-1);
	dtex_shader_program(-1);
}

}