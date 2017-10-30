#ifndef _EASYTRAIL_FILE_IO_H_
#define _EASYTRAIL_FILE_IO_H_

#include <string>
#include <memory>

namespace ee { class LibraryPanel; }
namespace s2 { class TrailEmitterCfg; }

namespace etrail
{

class MotionTrail;
class ToolbarPanel;

class FileIO
{
public:
	static void Store(const std::string& filepath, MotionTrail* mt,
		ToolbarPanel* toolbar);
	static void Load(const std::string& filepath, MotionTrail* mt,
		ToolbarPanel* toolbar, ee::LibraryPanel* library);

	static MotionTrail* LoadMT(const std::string& filepath);
	static std::shared_ptr<s2::TrailEmitterCfg> LoadMTConfig(const std::string& filepath);

}; // FileIO

}

#endif // _EASYTRAIL_FILE_IO_H_