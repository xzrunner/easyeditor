#ifndef _EPBIN_IMAGE_IDER_H_
#define _EPBIN_IMAGE_IDER_H_

#include <map>
#include <string>

namespace epbin
{

class ImageIDer
{
public:
	ImageIDer(const std::string& filepath);

	int Query(const std::string& path) const;

private:
	void Load(const std::string& filepath);

private:
	std::map<std::string, int> m_ids;

}; // ImageIDer

}

#endif // _EPBIN_IMAGE_IDER_H_