#ifndef _EASYDB_PACK_TEXTURE_H_
#define _EASYDB_PACK_TEXTURE_H_

#include "ICommand.h"

#include <JSON/json.h>
#include <wx/string.h>

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
	void Trigger(const std::string& config_path);

private:
	void GetSrcTexFile(const Json::Value& pkg_val, const wxString& config_dir,
		const wxString& src_folder, std::vector<wxString>& filepaths) const;

}; // PackTexture

}

#endif // _EASYDB_PACK_TEXTURE_H_