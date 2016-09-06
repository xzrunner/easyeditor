#ifndef _TEST_COMPLEX_H_
#define _TEST_COMPLEX_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestComplex : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	void InitGTxt();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestComplex

}

#endif // _TEST_COMPLEX_H_