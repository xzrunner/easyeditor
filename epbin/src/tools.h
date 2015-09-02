#ifndef _EPBIN_TOOLS_H_
#define _EPBIN_TOOLS_H_

#include <string.h>
#include <stdint.h>
#include <fstream>

namespace epbin
{

template<class T>
struct DeletePointerFunctor
{
	void operator()(const T* pT) const
	{
		delete pT, pT = NULL;
	}
};

template<typename T>
inline void pack2mem(const T& d, uint8_t** ptr) {
	memcpy(*ptr, &d, sizeof(d));
	*ptr += sizeof(d);
}

template<typename T>
inline void pack2file(const T& d, std::ofstream& fout) {
	fout.write(reinterpret_cast<const char*>(&d), sizeof(d));
}

inline void str_replace(std::string& str, const std::string& old_value, const std::string& new_value)   
{
	for(std::string::size_type pos = 0; pos != std::string::npos; pos += new_value.length()) {   
		if((pos = str.find(old_value,pos)) != std::string::npos) {
			str.replace(pos, old_value.length(), new_value);
		} else {
			break;
		}
	}   
}

inline std::string get_file_dir(const std::string& filepath) {
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	return filepath.substr(0, pos_divide);
}

inline int ceil4(int x) {
	return (x + 3) & ~3;
}

}

#endif // _EPBIN_TOOLS_H_