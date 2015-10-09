#ifndef _LIBTEXPACKER_GEN_REGULAR_RECT_IAMGE_H_
#define _LIBTEXPACKER_GEN_REGULAR_RECT_IAMGE_H_

#include <wx/wx.h>

namespace libtexpacker
{

class GenRegularRectImage
{
public:
	static void CreateMulti(const wxString& filepath);

private:
	static void CreateSingle(const wxString& filepath);

}; // GenRegularRectImage

}

#endif // _LIBTEXPACKER_GEN_REGULAR_RECT_IAMGE_H_