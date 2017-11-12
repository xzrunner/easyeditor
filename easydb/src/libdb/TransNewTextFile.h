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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransNewTextFile(); }

private:
	void Run(const std::string& folder);

}; // TransNewTextFile


}

#endif // _EASYDB_TRANS_NEW_TEXT_FILE_H_