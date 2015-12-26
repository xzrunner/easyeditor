#ifndef _EASYPARTICLE2D_FILE_IO_H_
#define _EASYPARTICLE2D_FILE_IO_H_

#include <string>

struct p2d_emitter_cfg;

namespace eparticle2d
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
	static p2d_emitter_cfg* LoadPSConfig(const std::string& filepath);

}; // FileIO

}

#endif // _EASYPARTICLE2D_FILE_IO_H_