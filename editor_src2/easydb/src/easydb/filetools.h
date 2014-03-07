#ifndef _EASYDB_MD5_H_
#define _EASYDB_MD5_H_

#define __alpha
extern "C" {
#include <md5.h>
};

namespace edb
{

extern "C" {

static char _hex[] = "0123456789abcdef";

int md5_file(const char* filename, char sig[32])
{
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		return 1;
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

	for(int i=0;i<16;i++) {
		sig[i*2] = _hex[decrypt[i] >> 4];
		sig[i*2+1] = _hex[decrypt[i] & 0xf];
	}

	return 0;
}

}; // extern "C"

}

#endif // _EASYDB_MD5_H_