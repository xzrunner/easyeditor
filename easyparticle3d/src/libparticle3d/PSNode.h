#ifndef _EASYPARTICLE3D_PS_NODE_H_
#define _EASYPARTICLE3D_PS_NODE_H_

#include <ee/SceneNode.h>

#include <SM_Matrix.h>
#include <sprite2/RenderColor.h>

struct p3d_sprite;

namespace eparticle3d
{

class RenderParams
{
public:
	sm::mat4 mat;
	s2::RenderColor ct;
	p3d_sprite* p3d;

	RenderParams() : p3d(NULL) {}
};

class PSNode : public ee::SceneNode
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

	static PSNode* Instance();

private:
	PSNode();

	void Init();

private:
	float m_time;

private:
	static PSNode* m_instance;

}; // PSNode

}

#endif // _EASYPARTICLE3D_PS_NODE_H_