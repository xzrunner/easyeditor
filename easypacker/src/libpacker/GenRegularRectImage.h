#ifndef _LIBPACKER_GEN_REGULAR_RECT_IAMGE_H_
#define _LIBPACKER_GEN_REGULAR_RECT_IAMGE_H_

#include <wx/wx.h>

namespace libpacker
{

class GenRegularRectImage
{
public:
	static void CreateMulti(const wxString& filepath);

private:
	static void CreateSingle(const wxString& filepath);

}; // GenRegularRectImage

}

#endif // _LIBPACKER_GEN_REGULAR_RECT_IAMGE_H_