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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransOldShapeFile(); }

private:
	void Run(const std::string& folder);

}; // TransOldShapeFile

}

#endif // _EASYDB_TRANS_OLD_SHAPE_FILE_H_