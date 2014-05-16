#ifndef _EASYDB_TRANS_TO_GIF_H_
#define _EASYDB_TRANS_TO_GIF_H_

#include <string>

namespace edb
{

class TransToGif
{
public:
	static void trigger(const std::string& srcdir, const std::string& dstdir);

}; // TransToGif

}

#endif // _EASYDB_TRANS_TO_GIF_H_