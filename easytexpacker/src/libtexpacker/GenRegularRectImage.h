#ifndef _EASYTEXPACKER_GEN_REGULAR_RECT_IAMGE_H_
#define _EASYTEXPACKER_GEN_REGULAR_RECT_IAMGE_H_

#include <string>

namespace etexpacker
{

class GenRegularRectImage
{
public:
	static void CreateMulti(const std::string& filepath);

private:
	static void CreateSingle(const std::string& filepath);

}; // GenRegularRectImage

}

#endif // _EASYTEXPACKER_GEN_REGULAR_RECT_IAMGE_H_