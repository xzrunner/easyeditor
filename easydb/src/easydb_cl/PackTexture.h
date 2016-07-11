#ifndef _EASYDB_PACK_TEXTURE_H_
#define _EASYDB_PACK_TEXTURE_H_

#include "ICommand.h"

#include <vector>

namespace etexpacker { class ImageTrimData; class NormalPack; }

namespace edb
{

class PackTexture : public ICommand
{
public:
	PackTexture() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	void Pack(const std::string& str_val);

	static void CompressPackedTex(const etexpacker::NormalPack& tp, int start_id,
		const std::string& file, const std::string& fmt, bool fast);
	
	static ICommand* Create() { return new PackTexture(); }

private:
	struct Package
	{
		std::vector<std::string> sources;
		std::vector<std::string> ignores;

		std::string format;
		std::string quality;

		int size_min, size_max;

		etexpacker::ImageTrimData* trim;

		int extrude_min, extrude_max;

		Package()
		{
			size_min = size_max = 0;
			trim = NULL;
			extrude_min = extrude_max = 0;
		}
	}; // Package

private:
	static void Pack(const std::vector<Package>& packages, const std::string& src_dir, const std::string& dst_file);
		
	static etexpacker::ImageTrimData* PreparePackages(const std::string& str, std::vector<Package>& packages, std::string& src, std::string& dst);
	static void PackPackage(const Package& pkg, const std::string& src_dir, const std::string& dst_file, int& start_id);

}; // PackTexture

}

#endif // _EASYDB_PACK_TEXTURE_H_