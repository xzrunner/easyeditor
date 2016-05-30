#ifndef _EASYEDITOR_WX_HELPER_H_
#define _EASYEDITOR_WX_HELPER_H_

#include <wx/arrstr.h>

#include <vector>

namespace ee
{

class WXHelper
{
public:

	static wxArrayString ToWXStringArray(const std::vector<std::string>& src);

}; // WXHelper

}

#endif // _EASYEDITOR_WX_HELPER_H_