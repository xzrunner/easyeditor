#ifndef _EASYDB_PACK_COCO_H_
#define _EASYDB_PACK_COCO_H_

#include "ICommand.h"

#include <json/json.h>
#include <wx/string.h>

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

private:
	void Trigger(const std::string& config_path);

private:
	void Prepare(const Json::Value& pkg_val, const wxString& config_dir);

	void PackTexture(const Json::Value& pkg_val, const wxString& config_dir,
		const libpacker::ImageTrimData& trim) const;
	void CompressTexture(const std::string& filepath, const std::string& type) const;
	void GetAllImageFiles(const Json::Value& pkg_val, const wxString& config_dir,
		const wxString& src_folder, std::vector<std::string>& images) const;

	void PackLuaFile(const Json::Value& pkg_val, const wxString& config_dir) const;
	void GetAllDataFiles(const wxString& src_folder, const wxString& filter, 
		std::vector<wxString>& files) const;

	void PackBinFiles(const Json::Value& pkg_val, const wxString& config_dir) const;
	void GetAllPTSFiles(const Json::Value& pkg_val, const wxString& config_dir,
		const wxString& src_folder, std::vector<std::string>& pts_files) const;

}; // PackCoco

}

#endif // _EASYDB_PACK_COCO_H_