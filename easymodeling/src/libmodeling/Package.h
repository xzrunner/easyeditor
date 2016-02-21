#ifndef _EASYMODELING_PACKAGE_H_
#define _EASYMODELING_PACKAGE_H_

#include <fstream>
#include <vector>

namespace emodeling
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
	static void PackBody(const Body& data, std::ofstream& fout);

	static void PackJoint(const Joint& data, std::ofstream& fout,
		const std::vector<Body*>& bodies);

private:
	static int QueryBodyIndex(const Body* body, 
		const std::vector<Body*>& bodies);

}; // Paskage

}

#endif // _EASYMODELING_PACKAGE_H_
