#ifndef _EASYDB_TRANS_OLD_SHAPE_FILE_H_
#define _EASYDB_TRANS_OLD_SHAPE_FILE_H_

#include "ICommand.h"



namespace edb
{

class TransOldShapeFile : public ICommand
{
public:
	TransOldShapeFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransOldShapeFile(); }

private:
	void Run(const std::string& folder);

}; // TransOldShapeFile

}

#endif // _EASYDB_TRANS_OLD_SHAPE_FILE_H_