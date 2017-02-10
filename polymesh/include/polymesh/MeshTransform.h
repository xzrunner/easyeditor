#ifndef _POLYMESH_MESH_TRANSFORM_H_
#define _POLYMESH_MESH_TRANSFORM_H_

#include <SM_Vector.h>

#include <vector>

namespace pm
{

class MeshTransform
{
public:	
	void Lerp(const MeshTransform& begin, const MeshTransform& end, float process);

	const std::vector<std::pair<int, sm::vec2> >& GetTrans() const { return m_trans; }
	void SetTrans(const std::vector<std::pair<int, sm::vec2> >& trans) { m_trans = trans; }

	void Clear() { m_trans.clear(); }

private:
	std::vector<std::pair<int, sm::vec2> > m_trans;

}; // MeshTransform

}

#endif // _POLYMESH_MESH_TRANSFORM_H_