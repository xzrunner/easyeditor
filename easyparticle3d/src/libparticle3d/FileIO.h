#ifndef _EASYPARTICLE3D_FILE_IO_H_
#define _EASYPARTICLE3D_FILE_IO_H_

#include <string>

struct p3d_ps_config;
class ParticleSystem;

namespace eparticle3d
{

class ParticleSystem;
class ToolbarPanel;

class FileIO
{
public:
	static void Store(const std::string& filepath, ToolbarPanel* toolbar);
	static void Load(const std::string& filepath, ParticleSystem* ps,
		ToolbarPanel* toolbar);

	static ParticleSystem* LoadPS(const std::string& filepath);
	static p3d_ps_config* LoadPSConfig(const std::string& filepath);

}; // FileIO

}

#endif // _EASYPARTICLE3D_FILE_IO_H_