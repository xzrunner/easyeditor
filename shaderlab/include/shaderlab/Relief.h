#ifndef _SHADERLAB_PARSER_RELIEF_H_
#define _SHADERLAB_PARSER_RELIEF_H_

#include "Filter.h"

#define STRINGIFY(A)  #A
#include "relief.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    relief
 */
class Relief : public Filter
{
public:
	Relief() : Filter("_col_relief_") {}
	
protected:
	virtual std::string GetBody() const {
		return relief_body;
	}

}; // Relief

}
}

#endif // _SHADERLAB_PARSER_RELIEF_H_