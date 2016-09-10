#ifndef _TEST_PARTICLE2D_H_
#define _TEST_PARTICLE2D_H_

#include "TestTask.h"

#include <vector>

namespace s2 { class Sprite; } 

namespace test
{

class TestParticle2d : public TestTask
{
public:
	virtual void Init();
	virtual void Resize(int width, int height);
	virtual void Draw() const;
	virtual void Update();

private:
	std::vector<s2::Sprite*> m_sprites;

}; // TestParticle2d

}

#endif // _TEST_PARTICLE2D_H_