#ifndef _EASYTERRAIN3D_DEMO_OCEAN_H_
#define _EASYTERRAIN3D_DEMO_OCEAN_H_

#include "IDemo.h"
#include "image.h"

#include <easy3d.h>

namespace ee { class Image; }

namespace eterrain3d
{

class DemoOcean : public IDemo
{
public:
	DemoOcean(e3d::Camera& cam);

	virtual void Load();
	virtual void Draw() const;

private:
	void Update(float dt) const;

	float GetHeight(float x, float y) const;

private:
	struct Wave
	{
		ee::vec2 pos, speed;
		float len;
	};

private:
	e3d::Camera& m_cam;

	ee::Image* m_image;

	mutable std::vector<Wave> m_waves;

	mutable ee::vec2 m_uv_base, m_uv_spd;

}; // DemoOcean

}

#endif // _EASYTERRAIN3D_DEMO_OCEAN_H_