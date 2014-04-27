#ifndef _EASYDB_CLIP_IMAGES_H_
#define _EASYDB_CLIP_IMAGES_H_

#include <string>

namespace edb
{

class ClipImages
{
public:
	ClipImages(const std::string& dir);

private:
	void Clip(const std::string& dir);

}; // ClipImages

}

#endif // _EASYDB_CLIP_IMAGES_H_