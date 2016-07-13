#ifndef _EASYRESPACKER_PACK_PVR_H_
#define _EASYRESPACKER_PACK_PVR_H_

#include "PackImage.h"

#include <stdint.h>

namespace erespacker
{

class PackPVR : public PackImage
{
public:
	PackPVR(bool fast);
	virtual ~PackPVR();

	virtual void Load(const std::string& filepath);
	virtual void Store(const std::string& filepath, float scale) const;

	static int CalTexSize(int type, int width, int height);

private:
	void Clear();

	void Store(std::ofstream& fout, uint8_t* buffer, int buf_sz,
		int width, int height) const;
	void StoreScaled(std::ofstream& fout, float scale) const;

private:
	bool m_fast;
	std::string m_base_path;

	int m_internal_format;
	int m_has_alpha;

	int m_width, m_height;

	int m_data_sz;
	uint8_t* m_buffer;

}; // PackPVR

}

#endif // _EASYRESPACKER_PACK_PVR_H_