// json改变某一项的数据

#ifndef _EASYDB_FIX_JSON_FILES_H_
#define _EASYDB_FIX_JSON_FILES_H_

#include <drag2d.h>

namespace edb
{

class FixJsonFiles
{
public:
	FixJsonFiles(const std::string& dir, d2d::FileNameParser::Type type);

	void Scale(const std::string& key, float times);

private:
	wxArrayString m_files;
	d2d::FileNameParser::Type m_type;

}; // FixJsonFiles

}

#endif // _EASYDB_FIX_JSON_FILES_H_