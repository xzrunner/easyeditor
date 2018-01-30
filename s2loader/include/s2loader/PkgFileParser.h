#ifndef _S2LOADER_PKG_FILE_PARSER_H_
#define _S2LOADER_PKG_FILE_PARSER_H_

#include <cu/cu_stl.h>

#include <stdint.h>

struct fs_file;

namespace s2loader
{

class PkgFileParser
{
public:
	static uint32_t GetEpeIdxOffset(fs_file* file);
	static uint32_t GetEptIdxOffset(fs_file* file);

	static void SetEPPath(fs_file* file, const CU_STR& filepath, int pkg_id);

}; // PkgFileParser

}

#endif // _S2LOADER_PKG_FILE_PARSER_H_
