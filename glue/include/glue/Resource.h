#ifndef _GLUE_RESOURCE_H_
#define _GLUE_RESOURCE_H_

#include <CU_RefCountObj.h>

#include <string>

namespace glue
{

class Resource : public cu::RefCountObj
{
public:
	virtual ~Resource() {}
	virtual bool LoadFromFile(const std::string& filepath) = 0;

}; // Resource

}

#endif // _GLUE_RESOURCE_H_