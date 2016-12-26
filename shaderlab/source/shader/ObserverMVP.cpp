#include "ObserverMVP.h"
#include "RenderShader.h"

#include <unirender/typedef.h>

namespace sl
{

ObserverMVP::ObserverMVP(RenderShader* shader)
	: m_shader(shader)
{
}

void ObserverMVP::SetModelview(const sm::mat4* mat)
{
	m_shader->SetUniform(m_modelview, ur::UNIFORM_FLOAT44, mat->x);
}

void ObserverMVP::SetProjection(const sm::mat4* mat)
{
	m_shader->SetUniform(m_projection, ur::UNIFORM_FLOAT44, mat->x);
}

}