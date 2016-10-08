#ifndef _EASYDB_FIX_LR_SPR_NAME_H_
#define _EASYDB_FIX_LR_SPR_NAME_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixLRSprName : public ICommand
{
public:
	FixLRSprName() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new FixLRSprName(); }

protected:
	virtual bool FixSprite(Json::Value& val) const;

private:
	void InitMapName();

	void Trigger(const std::string& dir) const;

private:
	std::vector<std::pair<std::string, std::string> > m_map2name;

}; // FixLRSprName 

}

#endif // _EASYDB_FIX_LR_SPR_NAME_H_