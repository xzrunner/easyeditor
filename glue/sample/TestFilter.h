#ifndef _TEST_FILTER_H_
#define _TEST_FILTER_H_

#include "TestTask.h"

#include <glue.h>

#include <vector>

namespace test
{

class TestFilter : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestFilter

}

#endif // _TEST_FILTER_H_