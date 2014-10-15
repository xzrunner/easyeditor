#include "ParametricSurface.h"

namespace e3d 
{

void ParametricSurface::SetInterval(const ParametricInterval& interval)
{
	_divisions = interval.divisions;
	_upper_bound = interval.upperBound;
	_texture_count = interval.texture_count;
	_slices = _divisions - ivec2(1, 1);
}

int ParametricSurface::GetVertexCount() const
{
	return _divisions.x * _divisions.y;
}

int ParametricSurface::GetTriangleIndexCount() const
{
	return 6 * _slices.x * _slices.y;
}

vec2 ParametricSurface::ComputeDomain(float x, float y) const
{
	return vec2(x * _upper_bound.x / _slices.x, y * _upper_bound.y / _slices.y);
}

void ParametricSurface::GenerateVertices(std::vector<float>& vertices,
										 unsigned char flags) const
{
	int floatsPerVertex = 3;
	if (flags & VertexFlagsNormals)
		floatsPerVertex += 3;
	if (flags & VertexFlagsTexCoords)
		floatsPerVertex += 2;

	vertices.resize(GetVertexCount() * floatsPerVertex);
	float* attribute = &vertices[0];

	for (int j = 0; j < _divisions.y; j++) {
		for (int i = 0; i < _divisions.x; i++) {

			// Compute Position
			vec2 domain = ComputeDomain(i, j);
			vec3 range = Evaluate(domain);
			attribute = range.Write(attribute);

			// Compute Normal
			if (flags & VertexFlagsNormals) {
				float s = i, t = j;

				// Nudge the point if the normal is indeterminate.
				if (i == 0) s += 0.01f;
				if (i == _divisions.x - 1) s -= 0.01f;
				if (j == 0) t += 0.01f;
				if (j == _divisions.y - 1) t -= 0.01f;

				// Compute the tangents and their cross product.
				vec3 p = Evaluate(ComputeDomain(s, t));
				vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
				vec3 v = Evaluate(ComputeDomain(s, t + 0.01f)) - p;
				vec3 normal = u.Cross(v).Normalized();
				if (InvertNormal(domain))
					normal = -normal;
				attribute = normal.Write(attribute);
			}

			// Compute Texture Coordinates
			if (flags & VertexFlagsTexCoords) {
				float s = _texture_count.x * i / _slices.x;
				float t = _texture_count.y * j / _slices.y;
				attribute = vec2(s, t).Write(attribute);
			}
		}
	}
}

void
ParametricSurface::GenerateTriangleIndices(std::vector<unsigned short>& indices) const
{
	indices.resize(GetTriangleIndexCount());
	std::vector<unsigned short>::iterator index = indices.begin();
	for (int j = 0, vertex = 0; j < _slices.y; j++) {
		for (int i = 0; i < _slices.x; i++) {
			int next = (i + 1) % _divisions.x;
			*index++ = vertex + i;
			*index++ = vertex + next;
			*index++ = vertex + i + _divisions.x;
			*index++ = vertex + next;
			*index++ = vertex + next + _divisions.x;
			*index++ = vertex + i + _divisions.x;
		}
		vertex += _divisions.x;
	}
}

}