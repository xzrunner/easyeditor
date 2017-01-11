#ifndef _EASYSPRPACKER_PACK_ARRAY_INL_
#define _EASYSPRPACKER_PACK_ARRAY_INL_

#include "binary_io.h"

#include <ee/Exception.h>

#include <easybuilder.h>
namespace lua = ebuilder::lua;

namespace esprpacker
{

template<typename T, typename TPack, typename TUnPack>
void PackArray<T, TPack, TUnPack >::
PackToLua(ebuilder::CodeGenerator& gen, const std::vector<T>& array, const std::string& name)
{
	lua::assign_with_end(gen, name + "_num", array.size());

	std::stringstream ss;
	ss << name << " = {";
	for (int i = 0, n = array.size(); i < n; ++i) {
		TPack v = static_cast<TPack>(array[i]);
		ss << v << ", ";
	}
	ss << "}";
	gen.line(ss.str());
}

template<typename T, typename TPack, typename TUnPack>
int PackArray<T, TPack,TUnPack>::
SizeOfUnpackFromBin(const std::vector<T>& array)
{
	return sizeof(TUnPack) * array.size();
}

template<typename T, typename TPack, typename TUnPack>
int PackArray<T, TPack,TUnPack>::
SizeOfPackToBin(const std::vector<T>& array)
{
	int sz = 0;
	sz += sizeof(uint16_t);					// number
	sz += sizeof(TPack) * array.size();		// array
	return sz;
}

template<typename T, typename TPack, typename TUnPack>
void PackArray<T, TPack,TUnPack>::
PackToBin(const std::vector<T>& array, uint8_t** ptr)
{
	if (array.size() > USHRT_MAX) {
		throw ee::Exception("PackArray::PackToBin num too large.");
	}

	uint16_t num = array.size();
	pack(num, ptr);

	for (int i = 0, n = array.size(); i < n; ++i) 
	{
		TPack v = static_cast<TPack>(array[i]);
		pack(v, ptr);
	}
}

}

#endif // _EASYSPRPACKER_PACK_ARRAY_INL_