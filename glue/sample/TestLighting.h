#ifndef _TEST_LIGHTING_H_
#define _TEST_LIGHTING_H_

#include "TestTask.h"

#include <glue.h>

struct m3_mesh;

namespace test
{

class TestLighting : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	m3_mesh* m_mesh;

	float m_angle;
	float m_z;

}; // TestLighting

}

#endif // _TEST_LIGHTING_H_