
#ifndef EPARTICLE_FILE_IO_H
#define EPARTICLE_FILE_IO_H

namespace eparticle
{
	class FileIO
	{
	public:
		static void store(const char* filepath);
		static void load(const char* filepath);

	}; // FileIO
}

#endif // EPARTICLE_FILE_IO_H