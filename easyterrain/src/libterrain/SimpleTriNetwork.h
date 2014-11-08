#ifndef _EASYTERRAIN_SIMPLE_TRI_NETWORK_H_
#define _EASYTERRAIN_SIMPLE_TRI_NETWORK_H_

#include "IDemo.h"

#include <easy3d.h>

namespace libterrain
{

typedef unsigned int GLuint;

class SimpleTriNetwork : public IDemo
{
public:
	SimpleTriNetwork(e3d::Camera& cam);
	
	virtual void Load();
	virtual void Draw() const;

private:
	// v1 - v2 is the long edge
	void DrawTriByLevel(const vec3& v0, const vec3& v1, const vec3& v2, int level) const;

	void GenTex();

private:
	e3d::Camera& m_cam;

	GLuint m_tex;

}; // SimpleTriNetwork

}

#endif // _EASYTERRAIN_SIMPLE_TRI_NETWORK_H_