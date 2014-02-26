#ifndef _EASYDB_MD5_H_
#define _EASYDB_MD5_H_

#define __alpha
extern "C" {
#include <md5.h>
};

namespace edb
{

extern "C" {

int md5_file(const char* filename, unsigned char digest[16])
{
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		return 1;
	}

	MD5_CTX ctx;
	unsigned char buffer[4096];

	MD5Init(&ctx);
	while(!feof(fp)) {
		size_t size = fread(buffer, sizeof(unsigned char), 4096, fp);
		MD5Update(&ctx, buffer, size);
	}
	MD5Final(digest, &ctx);

	fclose(fp);

	return 0;
}

}; // extern "C"

}

#endif // _EASYDB_MD5_H_