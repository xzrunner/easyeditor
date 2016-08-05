#ifndef _EASYTRAIL_FILE_IO_H_
#define _EASYTRAIL_FILE_IO_H_

namespace etrail
{

class FileIO
{
public:
	static void Store(const std::string& filepath, ParticleSystem* ps,
		ToolbarPanel* toolbar);
	static void Load(const std::string& filepath, ParticleSystem* ps,
		ToolbarPanel* toolbar, ee::LibraryPanel* library);

}; // FileIO

}

#endif // _EASYTRAIL_FILE_IO_H_