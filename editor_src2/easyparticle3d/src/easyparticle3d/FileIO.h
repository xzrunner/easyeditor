#ifndef _EASYPARTICLE3D_FILE_IO_H_
#define _EASYPARTICLE3D_FILE_IO_H_

namespace eparticle3d
{

class FileIO
{
public:
	static void store(const char* filepath);
	static void load(const char* filepath);

}; // FileIO

}

#endif // _EASYPARTICLE3D_FILE_IO_H_