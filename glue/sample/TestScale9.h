#ifndef _TEST_SCALE9_H_
#define _TEST_SCALE9_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestScale9 : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestScale9

}

#endif // _TEST_SCALE9_H_