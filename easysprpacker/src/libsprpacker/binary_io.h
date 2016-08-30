#ifndef _EASYSPRPACKER_BINARY_IO_H_
#define _EASYSPRPACKER_BINARY_IO_H_

#include <ee/Exception.h>

#include <fstream>
#include <string>

#include <stdint.h>
#include <assert.h>

namespace esprpacker
{

/************************************************************************/
/* common                                                               */
/************************************************************************/

template<typename T> inline 
void pack(const T& d, uint8_t** ptr) {
	memcpy(*ptr, &d, sizeof(d));
	*ptr += sizeof(d);
}

template<typename T> inline 
void unpack(T& d, uint8_t** ptr) {
	memcpy(&d, *ptr, sizeof(d));
	*ptr += sizeof(d);
}

template<typename T> inline 
void unpack(T& d, std::ifstream& fin) {
	fin.read(reinterpret_cast<char*>(&d), sizeof(d));
}

/************************************************************************/
/* string                                                               */
/************************************************************************/

inline 
int sizeof_pack_str(const std::string& str) {
	if (str.empty()) {
		return sizeof(uint8_t);
	} else {
		if (str.size() >= 255) {
			throw ee::Exception("%s is too long", str.c_str());
		}
		return sizeof(uint8_t) + str.size();
	}
}

inline 
int sizeof_unpack_str(const std::string& str) {
	if (str.empty()) {
		return 0;
	} else {
		if (str.size() >= 255) {
			throw ee::Exception("%s is too long", str.c_str());
		}
		int n = str.size();
		return (n+1+3) & ~3;
	}
}

inline 
void pack_str(const std::string& str, uint8_t** ptr) {
	if (str.empty()) {
		uint8_t c = 255;
		pack(c, ptr);
	} else {
		assert(str.size() < 255);
		uint8_t sz = str.size();
		pack(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			uint8_t c = str[i];
			pack(c, ptr);
		}
	}
}

inline 
void unpack_str(std::string& str, uint8_t** ptr) {
	uint8_t sz;
	unpack(sz, ptr);
	if (sz == 255) {
		;
	} else {
		for (int i = 0; i < sz; ++i) {
			uint8_t c;
			unpack(c, ptr);
			str.push_back((char)c);
		}
	}
}

}

#endif // _EASYSPRPACKER_BINARY_IO_H_