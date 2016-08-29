#ifndef _TEST_SPRITE_H_
#define _TEST_SPRITE_H_

#include "TestTask.h"

#include <glue.h>

#include <vector>

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
	std::vector<s2::Sprite*> m_sprites;

}; // TestSprite

}

#endif // _TEST_SPRITE_H_