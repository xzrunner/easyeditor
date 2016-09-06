#ifndef _TEST_LABEL_H_
#define _TEST_LABEL_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestLabel : public TestTask
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

}; // TestLabel

}

#endif // _TEST_LABEL_H_