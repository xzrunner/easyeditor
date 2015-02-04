#ifndef _EPBIN_BINARARY_B4R_H_
#define _EPBIN_BINARARY_B4R_H_

#include "typedef.h"

#include <string>
#include <vector>

namespace epbin
{

class BinaryB4R
{
public:
	BinaryB4R(const std::vector<std::string>& src_files, 
		const std::string& img_id_file);
	~BinaryB4R();

	void Pack(const std::string& outfile, bool compress) const;

private:
	struct Picture
	{
		int16_t w, h;
		int16_t x, y;

		std::string path;
		int16_t id;

		uint8_t* pixels;

		size_t flag_sz;
		uint8_t* flag;
		size_t block_used;

		size_t Size() const;
		void Store(uint8_t** ptr);

	}; // Picture

private:
	void LoadPictures(const std::vector<std::string>& src_files,
		const std::string& img_id_file);

	Picture* CreatePicture(const std::string& filepath) const;

private:
	std::vector<Picture*> m_pics;

}; // BinaryB4R

}

#endif // _EPBIN_BINARARY_B4R_H_