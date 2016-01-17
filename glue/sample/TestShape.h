#ifndef _TEST_SHAPE_H_
#define _TEST_SHAPE_H_

#include "TestTask.h"

namespace test
{

class TestShape : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

}; // TestShape

}

#endif // _TEST_SHAPE_H_