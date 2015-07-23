#ifndef _LIBPACKER_NORMAL_PACK_H_
#define _LIBPACKER_NORMAL_PACK_H_

#include <vector>
#include <map>
#include <wx/string.h>

#include "Rect.h"

namespace libpacker
{

class ImageTrimData;

class NormalPack
{
public:
	NormalPack(const std::vector<std::string>& files);

	void Pack();

	void OutputInfo(const std::string& dir, const ImageTrimData& trim,
		const std::string& dst_file) const;
	void OutputImage(const std::string& filepath) const;

private:
	const std::vector<std::string>& m_filepaths;

	std::vector<RectSize> m_src_sizes;

	std::vector<Rect> m_dst_pos;

	std::vector<libpacker::RectSize> m_dst_img_sz;

	std::vector<std::vector<int> > m_dst_img_idx;

	int m_extrude;

}; // NormalPack

}

#endif // _LIBPACKER_NORMAL_PACK_H_