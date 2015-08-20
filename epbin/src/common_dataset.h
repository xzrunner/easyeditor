#ifndef _EPBIN_COMMON_DATASET_H_
#define _EPBIN_COMMON_DATASET_H_

#include <string>
#include <stdint.h>

namespace epbin
{

class String
{
public:
	String();
	String(const std::string& str);

	void SetString(const std::string& str);

	size_t Size() const;

	void Store(uint8_t** ptr);

private:
	bool m_is_empty;

	std::string m_str;

}; // String

}

#endif // _EPBIN_COMMON_DATASET_H_