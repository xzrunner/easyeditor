#ifndef _EASYMODELING_JOINT_H_
#define _EASYMODELING_JOINT_H_

#include <ee/Object.h>

#include <string>

namespace ee { class Rect; class Sprite; }

namespace emodeling
{

class Body;

class Joint : public ee::Object
{
public:
	enum Type
	{
		e_revoluteJoint,
		e_prismaticJoint,
		e_distanceJoint,
		e_pulleyJoint,
		e_mouseJoint,
		e_gearJoint,
		e_wheelJoint,
		e_weldJoint,
		e_frictionJoint,
		e_ropeJoint,
		e_motorJoint
	};

	enum DrawType
	{
		e_default,
		e_mouseOn,
		e_selected
	};

public:
	Joint(Body* b0, Body* b1, Type type);
	virtual ~Joint() {}
	
	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void Draw(DrawType type) const = 0;

protected:
	void DrawBodyFlag() const;

	static sm::vec2 TransWorldToLocal(const sm::vec2& world, 
		const ee::Sprite* sprite);
	static sm::vec2 TransLocalToWorld(const sm::vec2& local, 
		const ee::Sprite* sprite);

private:
	void DrawBodyFlag(const sm::vec2& pos) const;

protected:
	static const float JOINT_RADIUS_OUT;
	static const float JOINT_RADIUS_IN;
	static const float JOINT_RADIUS_SELECT;

public:
	std::string m_name;

	Type m_type;

	Body* m_body_a;
	Body* m_body_b;

	bool m_collide_connected;

}; // Joint

}

#endif // _EASYMODELING_JOINT_H_
