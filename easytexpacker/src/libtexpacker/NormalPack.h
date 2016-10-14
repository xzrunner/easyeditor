#ifndef _EASYTEXPACKER_NORMAL_PACK_H_
#define _EASYTEXPACKER_NORMAL_PACK_H_

#include "Rect.h"
#include "typedef.h"

//#include <wx/string.h>

#include <vector>
#include <map>

namespace etexpacker
{

class ImageTrimData;

class NormalPack
{
public:
	NormalPack(const std::vector<std::string>& files, const ImageTrimData* trim_info,
		int extrude_min = 1, int extrude_max = 4, int start_id = 1);

	void Pack(int static_size, int max_size = 4096, int min_size = 0);

	void OutputInfo(const std::string& dir, const std::string& dst_file, 
		const std::string& format = "png") const;
	void OutputImage(const std::string& filepath) const;

	int DstTexCount() const { return m_dst_img_idx.size(); }
	int DstTexArea() const;

private:
	void Pack(PACK_STRATEGY strategy, int static_size, int max_size = 4096, int min_size = 0);

	void GetExtrude(const int bound[], int w, int h, int& left, int& right, int& bottom, int& up) const;
	int GetExtrude(int max, int tot, int edge) const;

private:
	const std::vector<std::string>& m_filepaths;

	const ImageTrimData* m_trim_info;

	std::vector<RectSize> m_src_sizes;

	std::vector<Rect> m_dst_pos;

	std::vector<RectSize> m_dst_img_sz;

	std::vector<std::vector<int> > m_dst_img_idx;

	int m_extrude_min, m_extrude_max;

	int m_start_id;

}; // NormalPack

}

#endif // _EASYTEXPACKER_NORMAL_PACK_H_