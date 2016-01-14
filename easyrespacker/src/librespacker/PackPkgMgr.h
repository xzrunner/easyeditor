#ifndef _LIBRESPACKER_PACK_PKG_MGR_H_
#define _LIBRESPACKER_PACK_PKG_MGR_H_

#include <drag2d.h>

namespace librespacker
{

class PackPkgMgr
{
public:
	PackPkgMgr(const std::string& filepath);
	~PackPkgMgr();

private:
	struct Package
	{
		std::string name;
		std::string path;
		int id;
		std::map<std::string, int> sprites;
	};

private:
	void LoadPackage(const Json::Value& val, const std::string& dir);

private:
	std::vector<Package*> m_pkgs;

}; // PackPkgMgr

}

#endif // _LIBRESPACKER_PACK_PKG_MGR_H_