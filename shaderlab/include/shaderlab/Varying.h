#ifndef _SHADERLAB_PARSER_VARYING_H_
#define _SHADERLAB_PARSER_VARYING_H_

#include "Variable.h"

namespace sl
{
namespace parser
{

class Varying : public Variable
{
public:
	Varying(const Variable& var);
	Varying(VariableType type, std::string name);

	virtual std::string& ToStatement(std::string& str) const;

}; // Varying

}
}

#endif // _SHADERLAB_PARSER_VARYING_H_