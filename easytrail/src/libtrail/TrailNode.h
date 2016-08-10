#ifndef _EASYTRAIL_TRAIL_NODE_H_
#define _EASYTRAIL_TRAIL_NODE_H_

#include <ee/SceneNode.h>

#include <SM_Matrix.h>
#include <sprite2/RenderColor.h>

struct t2d_emitter;

namespace etrail
{

class RenderParams
{
public:
	sm::mat4 mat;
	s2::RenderColor ct;
//	t2d_emitter* trail;

//	RenderParams() : trail(NULL) {}
};

class TrailNode : ee::SceneNode
{
public:
	//
	// interface ee::SceneNode
	//
	virtual bool Update(float dt);
	virtual void Draw() const;

	void UpdateTime();
	float GetTime() const { return m_time; }

	static TrailNode* Instance();

private:
	TrailNode();

	void Init();

private:
	float m_time;

private:
	static TrailNode* m_instance;

}; // TrailNode

}

#endif // _EASYTRAIL_TRAIL_NODE_H_