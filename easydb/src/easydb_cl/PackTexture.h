#ifndef _EASYDB_PACK_TEXTURE_H_
#define _EASYDB_PACK_TEXTURE_H_

#include "ICommand.h"

namespace libtexpacker { class ImageTrimData; }

namespace edb
{

class PackTexture : public ICommand
{
public:
	PackTexture() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackTexture(); }

private:
	void RunFromConfig(const std::string& cfg_file);
	void RunFromCmd(libtexpacker::ImageTrimData* trim, const std::string& src_dir, const std::string& dst_file,
		int static_size, int max_size, int min_size, int extrude_min, int extrude_max);

}; // PackTexture

}

#endif // _EASYDB_PACK_TEXTURE_H_