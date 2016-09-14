#ifndef _TEST_PACK_COMPLEX_H_
#define _TEST_PACK_COMPLEX_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestPackComplex : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestPackComplex

}

#endif // _TEST_PACK_COMPLEX_H_