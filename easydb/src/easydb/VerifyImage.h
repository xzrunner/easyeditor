#ifndef _EASYDB_VERIFY_IMAGE_H_
#define _EASYDB_VERIFY_IMAGE_H_

#include "ICommand.h"

#include <vector>
#include <map>
#include <set>
#include <wx/wx.h>

namespace edb 
{

class VerifyImage : public ICommand
{
public:
	VerifyImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new VerifyImage(); }

private:
	void Trigger(const std::string& dirpath);

	void InitFiles(const std::string& dirpath);

	void VerifyLack();
	void VerifySurplus();
	void Report() const;

	void HandleSpritePath(const std::string& filepath);

private:
	std::vector<std::string> _complex_files;
	std::vector<std::string> _anim_files;
	std::map<std::string, bool> _map_images;

	std::set<std::string> _reports;

	bool _delete_surplus;

}; // VerifyImages

}

#endif // _EASYDB_VERIFY_IMAGE_H_