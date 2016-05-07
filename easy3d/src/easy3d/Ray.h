#ifndef _EASY3D_RAY_H_
#define _EASY3D_RAY_H_

#include <ee/Matrix.h>

namespace e3d
{

class Ray
{
public:
//	Ray();
	Ray(const sm::vec3& start, const sm::vec3& dir);

	void Translate(const sm::vec3& offset);
//	void Rotate(const sm::mat4& mat);

	void ChangeCoordSystem(const sm::Quaternion& rot);

	const sm::vec3& Start() const;
	sm::vec3 Dir() const;

private:
	sm::vec3 m_start, m_dir_vec;
	sm::Quaternion m_dir;

}; // Ray

// inline Ray::Ray()
// 	: m_start(0, 0, 0)
// 	, m_dir(0, 0, 1, 1)
// {
// }

inline Ray::Ray(const sm::vec3& start, const sm::vec3& dir)
	: m_start(start)
	, m_dir_vec(dir)
{
}

inline void Ray::Translate(const sm::vec3& offset)
{
	m_start += offset;
}

// inline void Ray::Rotate(const sm::mat4& mat)
// {
// 	m_dir = mat * m_dir;
// }

inline const sm::vec3& Ray::Start() const 
{
	return m_start;
}

}

#endif // _EASY3D_RAY_H_