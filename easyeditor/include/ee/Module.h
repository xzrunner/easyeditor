#ifndef _EASYEDITOR_MODULE_H_
#define _EASYEDITOR_MODULE_H_

namespace ee
{

class Module
{
public:
	virtual ~Module() {}
	virtual const char* GetName() const = 0;
};

}

#endif // _EASYEDITOR_MODULE_H_