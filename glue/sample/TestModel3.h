#ifndef _TEST_MODEL3_H_
#define _TEST_MODEL3_H_

#include "TestTask.h"

#include <glue.h>

struct m3_model;

namespace test
{

class TestModel3 : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	void LoadParametricSurface();
	void LoadFromFile(const char* filepath);

private:
	m3_model* m_model;

	float m_angle;
	sm_vec3 m_pos;	

}; // TestModel3

}

#endif // _TEST_MODEL3_H_