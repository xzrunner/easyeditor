#ifndef _SHADERLAB_PARSER_ATTRIBUTE_H_
#define _SHADERLAB_PARSER_ATTRIBUTE_H_

#include "Variable.h"

namespace sl
{
namespace parser
{

class Attribute : public Variable
{
public:
	Attribute(const Variable& var);
	Attribute(VariableType type, std::string name);

	virtual std::string& ToStatement(std::string& str) const;

}; // Attribute

}
}

#endif // _SHADERLAB_PARSER_ATTRIBUTE_H_