#ifndef _EASYDB_CHANGE_TEXTURE_PACKER_JSON_FILE_H_
#define _EASYDB_CHANGE_TEXTURE_PACKER_JSON_FILE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class ChangeTPJsonFile : public ICommand
{
public:
	ChangeTPJsonFile() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new ChangeTPJsonFile(); }

private:
	void Run(const std::string& dir);

	void TranslateFrameXY(int dx, int dy);

private:
	wxArrayString m_files;

}; // ChangeTPJsonFile

}

#endif // _EASYDB_CHANGE_TEXTURE_PACKER_JSON_FILE_H_