#include "Package.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace libshape
{

void Paskage::packShape(d2d::Shape* shape, std::ofstream& fout)
{
	if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape))
	{
		int type = e_polygon;
		fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

		const std::vector<d2d::Vector>& vertices = poly->GetVertices();
		int size = vertices.size();
		fout.write(reinterpret_cast<const char*>(&size), sizeof(int));
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			fout.write(reinterpret_cast<const char*>(&vertices[i].x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&vertices[i].y), sizeof(float));
		}
	}
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
	{
		int type = e_circle;
		fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

		fout.write(reinterpret_cast<const char*>(&circle->center.x), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&circle->center.y), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&circle->radius), sizeof(float));
	}
}

}