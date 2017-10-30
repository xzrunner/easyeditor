#ifndef _EASYPARTICLE3D_FILE_IO_H_
#define _EASYPARTICLE3D_FILE_IO_H_

#include <sprite2/P3dEmitterCfg.h>

#include <string>
#include <memory>

namespace ee { class LibraryPanel; }

namespace eparticle3d
{

class ParticleSystem;
class ToolbarPanel;

class FileIO
{
public:
	static void Store(const std::string& filepath, ParticleSystem* ps,
		ToolbarPanel* toolbar);
	static void Load(const std::string& filepath, ParticleSystem* ps,
		ToolbarPanel* toolbar, ee::LibraryPanel* library);

	static ParticleSystem* LoadPS(const std::string& filepath);
	static s2::P3dEmitterCfgPtr LoadPSConfig(const std::string& filepath);

}; // FileIO

}

#endif // _EASYPARTICLE3D_FILE_IO_H_