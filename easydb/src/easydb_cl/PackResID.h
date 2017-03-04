#ifndef _EASYDB_PACK_RES_ID_H_
#define _EASYDB_PACK_RES_ID_H_

#include "ICommand.h"

#include <vector>

namespace edb
{

class PackResID : public ICommand
{
public:
	PackResID() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackResID(); }

	void Pack(const std::string& src, const std::string& dst);

private:
	void LoadPkgID(const std::string& src);
	void StorePkgID(const std::string& dst) const;

private:
	struct PkgID
	{
		std::string name;
		std::string path;
		int id;
	};

private:
	std::vector<PkgID> m_pkgs;

}; // PackResID

}

#endif // _EASYDB_PACK_RES_ID_H_