#ifndef _TEST_PACK_TRANS_H_
#define _TEST_PACK_TRANS_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestPackTrans : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestPackTrans

}

#endif // _TEST_PACK_TRANS_H_