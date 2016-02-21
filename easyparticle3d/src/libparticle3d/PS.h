#ifndef _EASYPARTICLE3D_PS_H_
#define _EASYPARTICLE3D_PS_H_

#include <ee/Matrix.h>
#include <ee/ColorTrans.h>
#include <ee/SceneNode.h>

struct p3d_sprite;

namespace eparticle3d
{

class RenderParams
{
public:
	ee::Matrix mat;
	ee::ColorTrans ct;
	p3d_sprite* p3d;

	RenderParams() : p3d(NULL) {}
};

class PS : public ee::SceneNode
{
public:
	//
	// interface ee::SceneNode
	//
	virtual bool Update(float dt);
	virtual void Draw() const;

	void BufferClear();

	void UpdateTime();
	float GetTime() const { return m_time; }

	static PS* Instance();

private:
	PS();

	void Init();

private:
	float m_time;

private:
	static PS* m_instance;

}; // PS

}

#endif // _EASYPARTICLE3D_PS_H_