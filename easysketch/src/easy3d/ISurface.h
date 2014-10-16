#ifndef _E3D_ISURFACE_H_
#define _E3D_ISURFACE_H_

#include <vector>

namespace e3d 
{

enum VertexFlags 
{
    VertexFlagsNormals = 1 << 0,
    VertexFlagsTexCoords = 1 << 1,
};

class ISurface 
{
public:
	virtual int GetVertexCount() const = 0;
	virtual int GetTriangleIndexCount() const = 0;
	virtual void GenerateVertices(std::vector<float>& vertices,
		unsigned char flags = 0) const = 0;
	virtual void GenerateTriangleIndices(std::vector<unsigned short>& indices) const = 0;
	virtual ~ISurface() {}
};

}

#endif // _E3D_ISURFACE_H_