#ifndef _EASYRESPACKER_UNPACKE_FROM_BIN_H_
#define _EASYRESPACKER_UNPACKE_FROM_BIN_H_

#include <string>
#include <vector>

#include <stdint.h>

namespace ee { class Image; }

namespace erespacker
{

class UnpackFromBin
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

private:
	static void Unpack(uint8_t** ptr, const std::vector<ee::Image*>& images);

}; // UnpackFromBin

}

#endif // _EASYRESPACKER_UNPACKE_FROM_BIN_H_