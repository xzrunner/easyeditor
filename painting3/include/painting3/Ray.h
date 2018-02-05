#pragma once

#include <SM_Vector.h>

namespace pt3
{

class Ray
{
public:
	Ray(const sm::vec3& start, const sm::vec3& dir)
		: m_start(start), m_dir(dir.Normalized()) {}

	const sm::vec3& Start() const { return m_start; }
	const sm::vec3& Dir() const { return m_dir; }

private:
	sm::vec3 m_start;
	sm::vec3 m_dir;

}; // Ray

}