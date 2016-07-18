#ifndef _EASYTRAIL_MTRAIL_H_
#define _EASYTRAIL_MTRAIL_H_

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
	t2d_emitter* trail;

	RenderParams() : trail(NULL) {}
};

class MTrail : ee::SceneNode
{
public:
	//
	// interface ee::SceneNode
	//
	virtual bool Update(float dt);
	virtual void Draw() const;

	void UpdateTime();
	float GetTime() const { return m_time; }

	static MTrail* Instance();

private:
	MTrail();

	void Init();

private:
	float m_time;

private:
	static MTrail* m_instance;

}; // MTrail

}

#endif // _EASYTRAIL_MTRAIL_H_