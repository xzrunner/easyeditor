#ifndef _LIBRESPACKER_RES_UNPACKER_H_
#define _LIBRESPACKER_RES_UNPACKER_H_

#include <drag2d.h>

namespace librespacker
{

class ResUnpacker
{
public:

	void UnpackLua(const std::string& filepath, 
		const std::vector<d2d::Image*>& images);

	void UnpackBin(const std::string& filepath, 
		const std::vector<d2d::Image*>& images);

}; // ResUnpacker

}

#endif // _LIBRESPACKER_RES_UNPACKER_H_