#ifndef _EASYDB_PACK_RES_H_
#define _EASYDB_PACK_RES_H_

#include "ICommand.h"

#include <json/json.h>
#include <wx/string.h>
#include <set>

namespace etexpacker { class ImageTrimData; }

namespace edb
{

class PackRes : public ICommand
{
public:
	PackRes() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackRes(); }

	void Pack(const std::string& str_val, const std::string& dir);
	void PackNew(const std::string& str_val, const std::string& dir);

private:
	void Prepare(const Json::Value& pkg_val, const std::string& config_dir);

	void PackTexture(const Json::Value& pkg_val, const std::string& config_dir) const;
	void GetAllImages(const Json::Value& pkg_val, const std::string& config_dir,
		std::vector<std::string>& images) const;
	void GetImagesFromCfg(const Json::Value& pkg_val, const std::string& config_dir,
		std::vector<std::string>& images) const;
	void GetImagesFromJson(const Json::Value& pkg_val, const std::string& config_dir, 
		std::vector<std::string>& images) const;
	void GetImagesFromJson(const std::vector<std::string>& src_dirs, const std::string& filepath, 
		std::set<std::string>& img_set) const;
	void GetImagesFromSprite(const std::vector<std::string>& src_dirs, const std::string& spr_dir, 
		const Json::Value& spr_val, std::set<std::string>& images) const;

	void PackLuaFile(const Json::Value& pkg_val, const std::string& config_dir) const;
	void GetAllDataFiles(const Json::Value& pkg_val, const std::string& config_dir, std::vector<std::string>& files) const;
	void AddJsonFile(const std::string& filepath, const std::string& filter, std::set<std::string>& json_set) const;

	void PackBinFiles(const Json::Value& pkg_val, const std::string& config_dir) const;
	void GetAllPTSFiles(const Json::Value& pkg_val, const std::string& config_dir,
		std::vector<std::string>& pts_files) const;

	// new packer
	void PackLuaAndBinFiles(const Json::Value& pkg_val, const std::string& config_dir, int LOD) const;
	void PackLuaAndBinFilesNew(const Json::Value& pkg_val, const std::string& config_dir, int LOD) const;

	std::string ConnectCfgDir(const std::string& cfg_dir, const std::string& path) const;

}; // PackRes

}

#endif // _EASYDB_PACK_RES_H_