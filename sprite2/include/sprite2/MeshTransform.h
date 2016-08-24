#ifndef _SPRITE2_MESH_TRANSFORM_H_
#define _SPRITE2_MESH_TRANSFORM_H_

#include <SM_Vector.h>

#include <map>

namespace s2
{

class Mesh;

class MeshTransform
{
public:
	void LoadFromMesh(const Mesh* mesh);
	void StoreToMesh(const Mesh* mesh) const;

	void SetTween(const MeshTransform& s, const MeshTransform& e, float process);

	const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& GetMap() const { return m_map; }
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& GetMap() { return m_map; }

private:
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp> m_map;

}; // MeshTransform

}

#endif // _SPRITE2_MESH_TRANSFORM_H_