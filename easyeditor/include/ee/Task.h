#ifndef _EASYEDITOR_TASK_H_
#define _EASYEDITOR_TASK_H_

#include <vector>

namespace ee
{

class Sprite;
class EditPanel;

class Task
{
public:
	virtual void Load(const char* filename) = 0;
	virtual void Store(const char* filename) const = 0;
	virtual bool IsDirty() const = 0;
	virtual void GetAllSprite(std::vector<const Sprite*>& sprites) const = 0;
	virtual const EditPanel* GetEditPanel() const = 0;
	virtual ~Task() {}
};

}

#endif // _EASYEDITOR_TASK_H_
