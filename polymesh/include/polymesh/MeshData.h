#ifndef _POLYMESH_MESH_DATA_H_
#define _POLYMESH_MESH_DATA_H_

#include <SM_Vector.h>
#include <CU_Uncopyable.h>

#include <vector>

namespace pm
{

class MeshTransform;

class MeshData : private cu::Uncopyable
{
public:
	virtual ~MeshData() {}
	virtual void Dump(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const = 0;
	virtual void LoadFromTransform(const MeshTransform& transform) = 0;
	virtual void StoreToTransform(MeshTransform& transform) const = 0;

}; // MeshData

}

#endif // _POLYMESH_MESH_DATA_H_