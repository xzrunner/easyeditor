#ifndef _EASYDB_UNIQUE_IMAGE_H_
#define _EASYDB_UNIQUE_IMAGE_H_

#include "ICommand.h"

#include <map>
#include <vector>

namespace edb
{

class UniqueImage : public ICommand
{
public:
	UniqueImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new UniqueImage(); }

private:
	void Trigger(const std::string& imgdir, const std::string& jsondir);

	void ProcessImageFiles(const std::string& imgdir);

	void ProcessJsonFiles(const std::string& jsondir);
	void FixImagePath(const std::string& animpath);

	void RemoveImages();

private:
	std::map<std::string, std::string> m_map_md5_2_image;
	std::map<std::string, std::string> m_map_image_2_md5;

	std::vector<std::string> m_to_remove;

}; // UniqueImage

}

#endif // _EASYDB_UNIQUE_IMAGE_H_