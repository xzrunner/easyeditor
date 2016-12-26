#ifndef _SHADERLAB_PARSER_OUTLINE_H_
#define _SHADERLAB_PARSER_OUTLINE_H_

#include "Filter.h"

#define STRINGIFY(A)  #A
#include "outline.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    outline
 */
class Outline : public Filter
{
public:
	Outline() : Filter("_col_outline_") {}
	
protected:
	virtual std::string GetBody() const {
		return outline_body;
	}

}; // Outline

}
}

#endif // _SHADERLAB_PARSER_OUTLINE_H_