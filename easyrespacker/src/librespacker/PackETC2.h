#ifndef _EASYRESPACKER_PACK_ETC2_H_
#define _EASYRESPACKER_PACK_ETC2_H_

#include "PackImage.h"

#include <stdint.h>

namespace erespacker
{

class PackETC2 : public PackImage
{
public:
	PackETC2(bool fast);
	virtual ~PackETC2();

	virtual void Load(const std::string& filepath);
	virtual void Store(const std::string& filepath, float scale) const;

private:
	void Clear();

	void Store(std::ofstream& fout, uint8_t* buffer, int width, int height) const;
	void StoreScaled(std::ofstream& fout, float scale) const;

private:
	bool m_fast;
	std::string m_base_path;

	int m_width, m_height;

	uint8_t* m_buf;	

}; // PackETC2

}

#endif // _EASYRESPACKER_PACK_ETC2_H_