#ifndef _EASYDB_TRANS_NEW_TEXT_FILE_H_
#define _EASYDB_TRANS_NEW_TEXT_FILE_H_

#include "ICommand.h"



namespace edb
{

class TransNewTextFile : public ICommand
{
public:
	TransNewTextFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransNewTextFile(); }

private:
	void Run(const std::string& folder);

}; // TransNewTextFile


}

#endif // _EASYDB_TRANS_NEW_TEXT_FILE_H_