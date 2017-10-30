#ifndef _EASYRESPACKER_RES_UNPACKER_H_
#define _EASYRESPACKER_RES_UNPACKER_H_

#include <ee/Image.h>

#include <vector>

namespace erespacker
{

class ResUnpacker
{
public:

	void UnpackLua(const std::string& filepath, 
		const std::vector<ee::ImagePtr>& images);

	void UnpackBin(const std::string& filepath, 
		const std::vector<ee::ImagePtr>& images);

}; // ResUnpacker

}

#endif // _EASYRESPACKER_RES_UNPACKER_H_