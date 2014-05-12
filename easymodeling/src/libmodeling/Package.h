
#ifndef LIBMODELING_PACKAGE_H
#define LIBMODELING_PACKAGE_H

#include <drag2d.h>

namespace libmodeling
{
	class Body;
	class Joint;

	class Paskage
	{
	public:
		enum ShapeType
		{
			e_circle = 0,
			e_rect,
			e_chain,
			e_polygon
		};

	public:
		static void packBody(const Body& data, std::ofstream& fout);

		static void packJoint(const Joint& data, std::ofstream& fout,
			const std::vector<Body*>& bodies);

	private:
		static int queryBodyIndex(const Body* body, 
			const std::vector<Body*>& bodies);

	}; // Paskage
}

#endif // LIBMODELING_PACKAGE_H
