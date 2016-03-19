#ifndef _TEST_IMAGE25_H_
#define _TEST_IMAGE25_H_

#include "TestTask.h"

#include <vector>

#include <glue.h>

namespace test
{

class TestImage25 : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<glue::Sprite*> m_sprites;
	
}; // TestImage25

}

#endif // _TEST_IMAGE25_H_