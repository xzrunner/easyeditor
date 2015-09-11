#ifndef _EASYCOCO_TOOLS_H_
#define _EASYCOCO_TOOLS_H_

#include <drag2d.h>

namespace libcoco
{

static const float SCALE = 16;

static const uint8_t TAG_ID			= 1;
static const uint8_t TAG_COLOR		= 2;
static const uint8_t TAG_ADDITIVE	= 4;
static const uint8_t TAG_MATRIX		= 8;
static const uint8_t TAG_TOUCH		= 16;
static const uint8_t TAG_MATRIXREF	= 32;
static const uint8_t TAG_COLMAP		= 64;

enum TextureType
{
	TT_PNG4,
	TT_PNG8,
	TT_PVR,
	TT_PKM
};

// template <class T>
// void endswap(T* objp)
// {
// 	unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
// 	std::reverse(memp, memp + sizeof(T));
// }

template<typename T>
inline void pack(const T& d, uint8_t** ptr) {
	memcpy(*ptr, &d, sizeof(d));
	*ptr += sizeof(d);
}

template<typename T>
inline void unpack(T& d, uint8_t** ptr) {
	memcpy(&d, *ptr, sizeof(d));
	*ptr += sizeof(d);
}

template<typename T>
inline void unpack(T& d, std::ifstream& fin) {
	fin.read(reinterpret_cast<char*>(&d), sizeof(d));
}

inline int sizeof_pack_str(const std::string& str) {
	if (str.empty()) {
		return sizeof(uint8_t);
	} else {
		if (str.size() >= 255) {
			throw d2d::Exception("%s is too long", str.c_str());
		}
		return sizeof(uint8_t) + str.size();
	}
}

inline int sizeof_unpack_str(const std::string& str) {
	if (str.empty()) {
		return 0;
	} else {
		if (str.size() >= 255) {
			throw d2d::Exception("%s is too long", str.c_str());
		}
		int n = str.size();
		return (n+1+3) & ~3;
	}
}

inline void pack_str(const std::string& str, uint8_t** ptr) {
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

inline void unpack_str(std::string& str, uint8_t** ptr) {
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

#endif // _EASYCOCO_TOOLS_H_