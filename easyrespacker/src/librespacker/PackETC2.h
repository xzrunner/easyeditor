#ifndef _EASYRESPACKER_PACK_ETC2_H_
#define _EASYRESPACKER_PACK_ETC2_H_

#include "PackImage.h"

#include <stdint.h>

namespace erespacker
{

class PackETC2 : public PackImage
{
public:
	PackETC2();
	virtual ~PackETC2();

	virtual void Load(const std::string& filepath);
	virtual void Store(const std::string& filepath, float scale) const;

private:
	void Clear();

	void LoadCompressed(const std::string& filename, uint8_t*& buf);

private:
	int m_width, m_height;

	uint8_t* m_buf;	

}; // PackETC2

}

#endif // _EASYRESPACKER_PACK_ETC2_H_