#ifndef _EPBIN_PKM_LOADER_H_
#define _EPBIN_PKM_LOADER_H_

#include "TexLoader.h"
#include "typedef.h"

namespace epbin
{

class PKMLoader : public TexLoader
{
public:
	PKMLoader();
	virtual ~PKMLoader();

	virtual void Load(const std::string& filepath);
	virtual void Store(std::ofstream& fout) const;

private:
	void LoadCompressed(const std::string& filename, uint8_t*& buf, int& width, int& height);

private:
	int m_width, m_height;

	uint8_t *m_rgb_buf, *m_alpha_buf;

}; // PKMLoader

}

#endif // _EPBIN_PKM_LOADER_H_