#ifndef _EASYANIM_FILE_IO_H_
#define _EASYANIM_FILE_IO_H_

#include <string>

namespace eanim
{

class FileIO
{
public:
	static void Load(const std::string& filepath);
	static void StoreSingle(const std::string& filepath);
	static void StoreTemplate(const std::string& filepath);
	static void Reload();

	static void StoreAsGif(const std::string& src, const std::string& dst);
	static void StoreAsPng(const std::string& src, const std::string& dst);

private:
	static std::string m_filepath;

}; // FileIO

}

#endif // _EASYANIM_FILE_IO_H_