#include "FilepathHelper.h"

#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>

namespace glue
{

std::string FilepathHelper::Relative(const std::string& base, const std::string& path)
{
	return boost::filesystem::relative(path, base).string();
}

std::string FilepathHelper::Absolute(const std::string& base, const std::string& path)
{
	return boost::filesystem::absolute(path, base).string();
}

std::string FilepathHelper::Dir(const std::string& path)
{
	return boost::filesystem::path(path).parent_path().string();
}

std::string FilepathHelper::Format(const std::string& path)
{	
	std::string ret = path;

	char from = '\\', to = '/';
	std::replace(ret.begin(), ret.end(), from, to);

	ret = boost::filesystem::canonical(ret).string();

	return ret;
}

}