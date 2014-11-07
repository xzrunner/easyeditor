#ifndef _E3D_PARAMETRIC_SURFACE_H_
#define _E3D_PARAMETRIC_SURFACE_H_

#include "ISurface.h"
#include "Vector.h"

namespace e3d 
{

struct ParametricInterval 
{
	ivec2 divisions;
	vec2 upperBound;
	vec2 texture_count;
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
	virtual vec3 Evaluate(const vec2& domain) const = 0;
	virtual bool InvertNormal(const vec2& domain) const { return false; }
private:
	vec2 ComputeDomain(float i, float j) const;
	ivec2 _slices;
	ivec2 _divisions;
	vec2 _upper_bound;
	vec2 _texture_count;
}; // ParametricSurface

}

#endif // _E3D_PARAMETRIC_SURFACE_H_