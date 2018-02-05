#pragma once

#include <SM_Matrix.h>

namespace pt3
{

class AABB
{
public:
	AABB();
	AABB(const sm::vec3& min, const sm::vec3& max);

	void MakeEmpty();

	void Combine(const sm::vec3& pos);

	void Translate(const sm::vec3& offset);
	void Rotate(const sm::mat4& mat);

	const sm::vec3& Max() const;
	const sm::vec3& Min() const;

private:
	sm::vec3 m_min, m_max;

}; // AABB

}

#include "AABB.inl"