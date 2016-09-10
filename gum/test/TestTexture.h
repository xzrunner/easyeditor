#ifndef _TEST_TEXTURE_H_
#define _TEST_TEXTURE_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestTexture : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestTexture

}

#endif // _TEST_TEXTURE_H_