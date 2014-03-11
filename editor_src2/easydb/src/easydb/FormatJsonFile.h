// 格式化json文件，读入再导出

#ifndef _EASYDB_FORMAT_JSON_FILE_H_
#define _EASYDB_FORMAT_JSON_FILE_H_

#include <string>

namespace edb
{

class FormatJsonFile
{
public:
	static void implement(const std::string& dir);

}; // FormatJsonFile

}

#endif // _EASYDB_FORMAT_JSON_FILE_H_