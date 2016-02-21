#ifndef _EASY3D_PARAMETRIC_SURFACE_H_
#define _EASY3D_PARAMETRIC_SURFACE_H_

#include "ISurface.h"

#include <ee/Vector3D.h>

namespace e3d 
{

struct ParametricInterval 
{
	ee::ivec2 divisions;
	ee::vec2 upperBound;
	ee::vec2 texture_count;
};

class ParametricSurface : public ISurface 
{
public:
	int GetVertexCount() const;
	int GetTriangleIndexCount() const;
	void GenerateVertices(std::vector<float>& vertices, unsigned char flags) const;
	void GenerateTriangleIndices(std::vector<unsigned short>& indices) const;
protected:
	void SetInterval(const ParametricInterval& interval);
	virtual ee::vec3 Evaluate(const ee::vec2& domain) const = 0;
	virtual bool InvertNormal(const ee::vec2& domain) const { return false; }
private:
	ee::vec2 ComputeDomain(float i, float j) const;
	ee::ivec2 _slices;
	ee::ivec2 _divisions;
	ee::vec2 _upper_bound;
	ee::vec2 _texture_count;
}; // ParametricSurface

}

#endif // _EASY3D_PARAMETRIC_SURFACE_H_