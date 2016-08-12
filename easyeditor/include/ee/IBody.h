#ifndef _EASYEDITOR_INTERFACE_BODY_H_
#define _EASYEDITOR_INTERFACE_BODY_H_

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <CU_Cloneable.h>
#include <CU_RefCountObj.h>

#include <Box2D/Box2D.h>

#include <string>

namespace ee
{

	class IBody : public cu::Cloneable, public cu::RefCountObj
{
public:
	virtual ~IBody();

	//
	// Cloneable interface
	//
	virtual IBody* Clone() const;

	virtual void GetRect(sm::rect& rect, const sm::vec2& position, float angle) const = 0;

	b2Body* GetBody() const;
	void SetBody(b2Body* body);

	const std::string& GetFilename() const;

	bool IsAlive() const;
	void SetAlive(bool isAlive);

	void SetDensity(float density);
	void SetFriction(float friction);
	void SetCollisionFilter(const b2Filter& filter);

	void SetTransform(const sm::vec2& pos, float angle);

protected:
	IBody() {}
	IBody(b2World* world, float scale = 1.0f);

protected:
	std::string m_filename;

	bool m_alive;

	b2World* m_world;
	b2Body* m_body;

private:
	float m_scale;

}; // IBody

inline b2Body* IBody::GetBody() const
{
	return m_body;
}

inline void IBody::SetBody(b2Body* body)
{
	m_body = body;
}

inline const std::string& IBody::GetFilename() const
{
	return m_filename;
}

inline bool IBody::IsAlive() const
{
	return m_alive;
}

inline void IBody::SetAlive(bool isAlive)
{
	m_alive = isAlive;
}

}

#endif // _EASYEDITOR_INTERFACE_BODY_H_