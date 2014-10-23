#ifndef _E3D_RAY_H_
#define _E3D_RAY_H_

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace e3d
{

class Ray
{
public:
	Ray();
	Ray(const vec3& start, const vec3& dir);

	void Translate(const vec3& offset);
	void Rotate(const mat4& mat);

	void ChangeCoordSystem(const Quaternion& rot);

	const vec3& Start() const;
	const vec3& Dir() const;

private:
	vec3 m_start, m_dir;

}; // Ray

inline Ray::Ray()
	: m_start(0, 0, 0)
	, m_dir(1, 0, 0)
{
}

inline Ray::Ray(const vec3& start, const vec3& dir)
	: m_start(start)
	, m_dir(dir)
{
}

inline void Ray::Translate(const vec3& offset)
{
	m_start += offset;
}

inline void Ray::Rotate(const mat4& mat)
{
	m_dir = mat * m_dir;
}

inline const vec3& Ray::Start() const 
{
	return m_start;
}

inline const vec3& Ray::Dir() const
{
	return m_dir;
}

}

#endif // _E3D_RAY_H_