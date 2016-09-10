#ifndef _TEST_ANIM_H_
#define _TEST_ANIM_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestAnim : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestAnim

}

#endif // _TEST_ANIM_H_