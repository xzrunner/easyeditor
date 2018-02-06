#ifndef _EASYTERRAIN3D_DEMO_TRI_NETWORK_H_
#define _EASYTERRAIN3D_DEMO_TRI_NETWORK_H_

#include "IDemo.h"

#include <SM_Vector.h>

#include <vector>

namespace n3 { class Camera; }

namespace eterrain3d
{

typedef unsigned int GLuint;

class DemoTriNetwork : public IDemo
{
public:
	DemoTriNetwork(pt3::Camera& cam);
	
	virtual void Load() override;
	virtual void Draw() const override;

private:
	// v1 - v2 is the long edge
	void DrawTriByLevel(const sm::vec3& v0, const sm::vec3& v1, const sm::vec3& v2, int level) const;

	void GenTex();

	float GetRandHeight(const sm::vec3& v1, const sm::vec3& v2, int level) const;

private:
	pt3::Camera& m_cam;

	GLuint m_tex;

	std::vector<float> m_level2size;

}; // DemoTriNetwork

}

#endif // _EASYTERRAIN3D_DEMO_TRI_NETWORK_H_