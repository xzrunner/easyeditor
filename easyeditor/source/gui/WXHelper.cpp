#include "WXHelper.h"

namespace ee
{

wxArrayString WXHelper::ToWXStringArray(const std::vector<std::string>& src)
{
	wxArrayString ret;
	for (int i = 0, n = src.size(); i < n; ++i) {
		ret.Add(src[i]);
	}
	return ret;
}

}