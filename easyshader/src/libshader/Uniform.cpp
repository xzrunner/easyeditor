#include "Uniform.h"

#include <gl/gLee.h>

namespace eshader
{

Uniform::Uniform(const std::string& name, UniformType type)
	: m_name(name)
	, m_type(type)
	, m_location(0)
{
}

void Uniform::Load()
{
	switch (m_type) 
	{
		case UT_INT: case UT_BOOL: 
			glUniform1i(m_location, m_value.i[0]); 
			break;
		case UT_FLOAT: 
			glUniform1f(m_location, m_value.f[0]); 
			break;

		case UT_IVEC2: 
			glUniform2iv(m_location, 1, (int*) m_value.i); 
			break;
		case UT_IVEC3: 
			glUniform3iv(m_location, 1, (int*) m_value.i); 
			break;
		case UT_IVEC4: 
			glUniform4iv(m_location, 1, (int*) m_value.i); 
			break;
		case UT_VEC2: 
			glUniform2fv(m_location, 1, (float*) m_value.f); 
			break;
		case UT_VEC3: 
			glUniform3fv(m_location, 1, (float*) m_value.f); 
			break;
		case UT_VEC4: 
			glUniform4fv(m_location, 1, (float*) m_value.f); 
			break;

		case UT_MAT2: 
			glUniformMatrix2fv(m_location, 1, GL_FALSE, (float*) m_value.f); 
			break;
		case UT_MAT3: 
			glUniformMatrix3fv(m_location, 1, GL_FALSE, (float*) m_value.f); 
			break;
		case UT_MAT4: 
			glUniformMatrix4fv(m_location, 1, GL_FALSE, (float*) m_value.f); 
			break;
	}
}

bool Uniform::SetLocation(GLuint program)
{
	m_location = glGetUniformLocation(program, m_name.c_str());
	return m_location != -1;
}

void Uniform::Set(float x)
{
	m_value.f[0] = x;
}

void Uniform::Set(float x, float y)
{
	m_value.f[0] = x;
	m_value.f[1] = y;
}

void Uniform::Set(float x, float y, float z)
{
	m_value.f[0] = x;
	m_value.f[1] = y;
	m_value.f[2] = z;
}

void Uniform::Set(float x, float y, float z, float w)
{
	m_value.f[0] = x;
	m_value.f[1] = y;
	m_value.f[2] = z;
	m_value.f[3] = w;
}

void Uniform::Set(int i)
{
    m_value.i[0] = i;
}

void Uniform::Set(int i, int j)
{
    m_value.i[0] = i;
    m_value.i[1] = j;
}

void Uniform::Set(int i, int j, int k)
{
    m_value.i[0] = i;
    m_value.i[1] = j;
    m_value.i[2] = k;
}

void Uniform::Set(int i, int j, int k, int h)
{
    m_value.i[0] = i;
    m_value.i[1] = j;
    m_value.i[2] = k;
    m_value.i[3] = h;
}

}