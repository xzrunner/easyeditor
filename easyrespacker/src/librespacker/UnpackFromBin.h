#ifndef _EASYRESPACKER_UNPACKE_FROM_BIN_H_
#define _EASYRESPACKER_UNPACKE_FROM_BIN_H_

#include <ee/Image.h>

#include <string>
#include <vector>

#include <stdint.h>

namespace erespacker
{

class UnpackFromBin
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<ee::ImagePtr>& images);

private:
	static void Unpack(uint8_t** ptr, const std::vector<ee::ImagePtr>& images);

}; // UnpackFromBin

}

#endif // _EASYRESPACKER_UNPACKE_FROM_BIN_H_