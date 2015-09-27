#include "EJShader.h"

#include <dtex.h>

namespace eejoy2d
{

void EJShader::Reset()
{
	dtex_shader_texture(0);
	dtex_shader_program(PROGRAM_NULL);
}

}