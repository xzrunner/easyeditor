#ifndef _TEST_IMAGE_H_
#define _TEST_IMAGE_H_

#include "TestTask.h"

#include <glue.h>

#include <vector>

namespace test
{

class TestImage : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<glue::Sprite*> m_sprites;

}; // TestImage

}

#endif // _TEST_IMAGE_H_