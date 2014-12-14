#ifndef _ESHADER_UNIFORM_H_
#define _ESHADER_UNIFORM_H_

#include <string>

typedef unsigned int GLuint;

namespace eshader 
{

union UniformValue
{
	float f[16];
	int i[16];
};

enum UniformType
{
	UT_INT = 1,
	UT_BOOL,
	UT_FLOAT,
	UT_IVEC2,
	UT_IVEC3,
	UT_IVEC4,
	UT_VEC2,
	UT_VEC3,
	UT_VEC4,
	UT_MAT2,
	UT_MAT3,
	UT_MAT4,

	UT_TIME
};

class Uniform
{
public:
	Uniform(const std::string& name, UniformType type);

	void Load();

	bool SetLocation(GLuint program);

	void Set(float x);
	void Set(float x, float y);
	void Set(float x, float y, float z);
	void Set(float x, float y, float z, float w);
	void Set(int i);
	void Set(int i, int j);
	void Set(int i, int j, int k);
	void Set(int i, int j, int k, int h);

	UniformType GetType() const { return m_type; }

private:
	std::string m_name;
	UniformType m_type;

	UniformValue m_value;
	UniformValue m_val_min, m_val_max;

	int m_location;

}; // Uniform

}

#endif // _ESHADER_UNIFORM_H_