#ifndef _EASYUI_ISTAGE_PAGE_H_
#define _EASYUI_ISTAGE_PAGE_H_

#include <drag2d.h>

namespace eui
{

class IStagePage
{
public:
	virtual void OnSelected() = 0;
}; // IStagePage

}

#endif // _EASYUI_ISTAGE_PAGE_H_