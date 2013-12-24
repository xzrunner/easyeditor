
#include "Package.h"

using namespace libshape;

void Paskage::packShape(d2d::IShape* shape, std::ofstream& fout)
{
	if (d2d::PolygonShape* poly = dynamic_cast<d2d::PolygonShape*>(shape))
	{
		int type = e_polygon;
		fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

		const std::vector<d2d::Vector>& vertices = poly->getVertices();
		int size = vertices.size();
		fout.write(reinterpret_cast<const char*>(&size), sizeof(int));
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			fout.write(reinterpret_cast<const char*>(&vertices[i].x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&vertices[i].y), sizeof(float));
		}
	}
	else if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		int type = e_circle;
		fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

		fout.write(reinterpret_cast<const char*>(&circle->center.x), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&circle->center.y), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&circle->radius), sizeof(float));
	}
}