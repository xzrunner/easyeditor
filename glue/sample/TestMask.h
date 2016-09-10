#ifndef _TEST_MASK_H_
#define _TEST_MASK_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestMask : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // _TEST_MASK_H_

}

#endif // _TEST_TRAIL_H_