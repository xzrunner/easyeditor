#ifndef _TEST_MULTI_H_
#define _TEST_MULTI_H_

#include "TestTask.h"

#include <vector>

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
	std::vector<glue::Sprite*> m_sprites;
	
}; // TestMulti

}

#endif // _TEST_MULTI_H_