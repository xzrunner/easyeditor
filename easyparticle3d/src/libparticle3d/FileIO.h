#ifndef _EASYPARTICLE3D_FILE_IO_H_
#define _EASYPARTICLE3D_FILE_IO_H_

namespace eparticle3d
{

class ToolbarPanel;
class ParticleSystem;

class FileIO
{
public:
	static void Store(const char* filepath, ToolbarPanel* toolbar);
	static void Load(const char* filepath, ParticleSystem* ps,
		ToolbarPanel* toolbar);

	static ParticleSystem* LoadPS(const char* filepath);

	static ps_cfg_3d* LoadPSConfig(const char* filepath);

}; // FileIO

}

#endif // _EASYPARTICLE3D_FILE_IO_H_