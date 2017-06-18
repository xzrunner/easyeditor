#include "MD5Helper.h"

#define __alpha
extern "C" {
#include <md5.h>
};

namespace edb
{

static const std::string HEX = "0123456789abcdef";

bool MD5Helper::File(const std::string& filepath, std::string& sig)
{
	FILE* fp = fopen(filepath.c_str(), "rb");
	if (!fp) {
		return false;
	}

	MD5_CTX md5;
	unsigned char buffer[4096];
	unsigned char decrypt[16];

	MD5Init(&md5);
	while(!feof(fp)) {
		size_t size = fread(buffer, sizeof(unsigned char), 4096, fp);
		MD5Update(&md5, buffer, size);
	}
	MD5Final(decrypt, &md5);

	fclose(fp);

	for(int i=0; i < 16; ++i) 
	{
		sig[i*2]   = HEX[decrypt[i] >> 4];
		sig[i*2+1] = HEX[decrypt[i] & 0xf];
	}

	return true;
}

}