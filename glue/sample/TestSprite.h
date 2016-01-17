#ifndef _TEST_SPRITE_H_
#define _TEST_SPRITE_H_

#include "TestTask.h"

#include <glue.h>

namespace test
{

class TestSprite : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	glue::Sprite* m_spr;
	
}; // TestSprite

}

#endif // _TEST_SPRITE_H_