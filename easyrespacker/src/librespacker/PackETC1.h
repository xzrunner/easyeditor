#ifndef _EASYRESPACKER_PACK_ETC1_H_
#define _EASYRESPACKER_PACK_ETC1_H_

#include "PackImage.h"

#include <stdint.h>

namespace erespacker
{

class PackETC1 : public PackImage
{
public:
	PackETC1();
	virtual ~PackETC1();

	virtual void Load(const std::string& filepath);
	virtual void Store(const std::string& filepath, float scale) const;

private:
	void Clear();

	void LoadCompressed(const std::string& filename, uint8_t*& buf, int& width, int& height);

private:
	int m_width, m_height;

	uint8_t *m_rgb_buf, *m_alpha_buf;

}; // PackPKM

}

#endif // _EASYRESPACKER_PACK_ETC1_H_