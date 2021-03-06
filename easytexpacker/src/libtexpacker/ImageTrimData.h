#ifndef _EASYTEXPACKER_IMAGE_TRIM_DATA_H_
#define _EASYTEXPACKER_IMAGE_TRIM_DATA_H_

#include <string>
#include <map>

namespace etexpacker
{

class ImageTrimData
{
public:
	struct Trim
	{
		int x, y, w, h;
		int ori_w, ori_h;

		int bound[8];
	};

public:
	ImageTrimData(const std::string& filepath);

	const Trim* Query(const std::string& filepath) const;

private:
	void Load(const std::string& filepath);

private:
	std::map<std::string, Trim> m_trim_info;

}; // ImageTrimData

}

#endif // _EASYTEXPACKER_IMAGE_TRIM_DATA_H_