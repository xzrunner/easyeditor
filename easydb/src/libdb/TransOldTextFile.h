#ifndef _EASYDB_TRANS_OLD_TEXT_FILE_H_
#define _EASYDB_TRANS_OLD_TEXT_FILE_H_

#include "ICommand.h"

#include <json/json.h>

namespace edb
{

class TransOldTextFile : public ICommand
{
public:
	TransOldTextFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransOldTextFile(); }

private:
	void Run(const std::string& folder);

private:
	void TransComplex(const std::string& filepath) const;

	bool TransSprite(Json::Value& value) const;

}; // TransOldTextFile


}

#endif // _EASYDB_TRANS_OLD_TEXT_FILE_H_