#ifndef _S2LOADER_RESOURCE_H_
#define _S2LOADER_RESOURCE_H_

namespace bimp { class FilePath; }

namespace s2loader
{

class Resource
{
public:
	virtual ~Resource() {}
	virtual bool LoadFromFile(int pkg_id, const bimp::FilePath& res_path, bool async) = 0;

}; // Resource

}

#endif // _S2LOADER_RESOURCE_H_
