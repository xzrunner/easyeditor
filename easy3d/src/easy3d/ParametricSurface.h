#ifndef _EASY3D_PARAMETRIC_SURFACE_H_
#define _EASY3D_PARAMETRIC_SURFACE_H_

#include "ISurface.h"

namespace e3d 
{

struct ParametricInterval 
{
	sm::ivec2 divisions;
	sm::vec2 upperBound;
	sm::vec2 texture_count;
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
	virtual sm::vec3 Evaluate(const sm::vec2& domain) const = 0;
	virtual bool InvertNormal(const sm::vec2& domain) const { return false; }
private:
	sm::vec2 ComputeDomain(float i, float j) const;
	sm::ivec2 _slices;
	sm::ivec2 _divisions;
	sm::vec2 _upper_bound;
	sm::vec2 _texture_count;
}; // ParametricSurface

}

#endif // _EASY3D_PARAMETRIC_SURFACE_H_