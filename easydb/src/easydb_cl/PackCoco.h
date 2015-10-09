#ifndef _EASYDB_PACK_COCO_H_
#define _EASYDB_PACK_COCO_H_

#include "ICommand.h"

#include <json/json.h>
#include <wx/string.h>
#include <set>

namespace libpacker { class ImageTrimData; }

namespace edb
{

class PackCoco : public ICommand
{
public:
	PackCoco() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackCoco(); }

	void Trigger(const std::string& config_path);

private:
	void Prepare(const Json::Value& pkg_val, const std::string& config_dir);

	void PackTexture(const Json::Value& pkg_val, const std::string& config_dir,
		const libtp::ImageTrimData* trim) const;
	void CompressTexture(const std::string& filepath, const std::string& type) const;
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

	void PackLuaAndBinFiles(const Json::Value& pkg_val, const std::string& config_dir) const;

	std::string ConnectCfgDir(const std::string& cfg_dir, const std::string& path) const;

}; // PackCoco

}

#endif // _EASYDB_PACK_COCO_H_