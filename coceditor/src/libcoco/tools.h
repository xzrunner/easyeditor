#ifndef _EASYCOCO_TOOLS_H_
#define _EASYCOCO_TOOLS_H_

#include <drag2d.h>

namespace libcoco
{

static const float SCALE = 16;

template<typename T>
inline void pack2mem(const T& d, uint8_t** ptr) {
	memcpy(*ptr, &d, sizeof(d));
	*ptr += sizeof(d);
}

int sizeof_pack_str(const std::string& str) {
	if (str.empty()) {
		return sizeof(uint8_t);
	} else {
		if (str.size() >= 255) {
			throw d2d::Exception("%s is too long", str.c_str());
		}
		return sizeof(uint8_t) + str.size();
	}
}

void pack_str2mem(const std::string& str, uint8_t** ptr) {
	if (str.empty()) {
		uint8_t c = 255;
		pack2mem(c, ptr);
	} else {
		assert(str.size() < 255);
		uint8_t sz = str.size();
		pack2mem(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			uint8_t c = str[i];
			pack2mem(c, ptr);
		}
	}
}

}

#endif // _EASYCOCO_TOOLS_H_