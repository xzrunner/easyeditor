#ifndef _EASYRESPACKER_RES_UNPACKER_H_
#define _EASYRESPACKER_RES_UNPACKER_H_

#include <vector>

namespace ee { class Image; }

namespace erespacker
{

class ResUnpacker
{
public:

	void UnpackLua(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

	void UnpackBin(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

}; // ResUnpacker

}

#endif // _EASYRESPACKER_RES_UNPACKER_H_