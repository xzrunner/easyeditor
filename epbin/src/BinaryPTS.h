#ifndef _EPBIN_BINARARY_PTS_H_
#define _EPBIN_BINARARY_PTS_H_

#include <stdint.h>
#include <vector>

namespace epbin
{

class BinaryPTS
{
public:
	BinaryPTS(const std::string& config_dir, const std::vector<std::string>& json_file, 
		const std::string& img_id_file);
	~BinaryPTS();

	void Pack(const std::string& outfile, bool compress) const;	

private:
	void Load(const std::string& source_folder, const std::vector<std::string>& json_file, 
		const std::string& img_id_file);

private:
	struct Point
	{
		int16_t x, y;
	};

	struct PTS
	{
		std::vector<std::vector<Point> > strips;

		int16_t id;

		size_t Size() const;
		void Store(uint8_t** ptr);
	};

private:
	std::vector<PTS*> m_pts;

}; // BinaryPTS

}

#endif // _EPBIN_BINARARY_PTS_H_