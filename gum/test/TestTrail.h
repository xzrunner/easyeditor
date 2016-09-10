#ifndef _TEST_TRAIL_H_
#define _TEST_TRAIL_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestTrail : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestTrail

}

#endif // _TEST_TRAIL_H_