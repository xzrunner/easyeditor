#ifndef _TEST_TASK_H_
#define _TEST_TASK_H_

namespace test
{

class TestTask
{
public:
	virtual ~TestTask() {}
	virtual void Init() = 0;
	virtual void Resize(int width, int height) = 0;
	virtual void Draw() const = 0;
	virtual void Update() = 0;
}; // TestTask

}

#endif // _TEST_TASK_H_