#ifndef _TEST_MULTI_H_
#define _TEST_MULTI_H_

#include "TestTask.h"

#include <glue.h>

namespace test
{

class TestMulti : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	glue::Sprite* m_spr;
	
}; // TestMulti

}

#endif // _TEST_MULTI_H_