#ifndef _EASY3D_RAY_H_
#define _EASY3D_RAY_H_

#include <ee/Vector3D.h>
#include <ee/Matrix.h>
#include <ee/Quaternion.h>

namespace e3d
{

class Ray
{
public:
//	Ray();
	Ray(const ee::vec3& start, const ee::vec3& dir);

	void Translate(const ee::vec3& offset);
//	void Rotate(const ee::mat4& mat);

	void ChangeCoordSystem(const ee::Quaternion& rot);

	const ee::vec3& Start() const;
	ee::vec3 Dir() const;

private:
	ee::vec3 m_start, m_dir_vec;
	ee::Quaternion m_dir;

}; // Ray

// inline Ray::Ray()
// 	: m_start(0, 0, 0)
// 	, m_dir(0, 0, 1, 1)
// {
// }

inline Ray::Ray(const ee::vec3& start, const ee::vec3& dir)
	: m_start(start)
	, m_dir_vec(dir)
{
}

inline void Ray::Translate(const ee::vec3& offset)
{
	m_start += offset;
}

// inline void Ray::Rotate(const ee::mat4& mat)
// {
// 	m_dir = mat * m_dir;
// }

inline const ee::vec3& Ray::Start() const 
{
	return m_start;
}

}

#endif // _EASY3D_RAY_H_