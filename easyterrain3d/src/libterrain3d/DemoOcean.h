#ifndef _EASYTERRAIN3D_DEMO_OCEAN_H_
#define _EASYTERRAIN3D_DEMO_OCEAN_H_

#include "IDemo.h"
#include "image.h"

#include <SM_Vector.h>

#include <vector>
#include <memory>

namespace ee { class Image; }
namespace n3 { class Camera; }

namespace eterrain3d
{

class DemoOcean : public IDemo
{
public:
	DemoOcean(n3::Camera& cam);

	virtual void Load() override;
	virtual void Draw() const override;

private:
	void Update(float dt) const;

	float GetHeight(float x, float y) const;

private:
	struct Wave
	{
		sm::vec2 pos, speed;
		float len;
	};

private:
	n3::Camera& m_cam;

	std::shared_ptr<ee::Image> m_image;

	mutable std::vector<Wave> m_waves;

	mutable sm::vec2 m_uv_base, m_uv_spd;

}; // DemoOcean

}

#endif // _EASYTERRAIN3D_DEMO_OCEAN_H_