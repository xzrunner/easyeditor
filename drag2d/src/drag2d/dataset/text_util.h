#ifndef _DRAG2D_TEXT_UTIL_H_
#define _DRAG2D_TEXT_UTIL_H_

#include <string>
#include <vector>
#include <wx/wx.h>

namespace d2d
{

std::string string2utf8(const std::string& str);

void utf8_to_unicode(const char* utf8_src, 
					 std::vector<int>& unicode_dst,
					 std::vector<wxString>& utf8_dst);

}

#endif // _DRAG2D_TEXT_UTIL_H_