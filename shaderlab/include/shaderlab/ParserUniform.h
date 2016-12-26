#ifndef _SHADERLAB_PARSER_UNIFORM_H_
#define _SHADERLAB_PARSER_UNIFORM_H_

#include "Variable.h"

namespace sl
{
namespace parser
{

class Uniform : public Variable
{
public:
	Uniform(const Variable& var);
	Uniform(VariableType type, std::string name);

	virtual std::string& ToStatement(std::string& str) const;
	
private:
	float m_value[16];

}; // Uniform

}
}

#endif // _SHADERLAB_PARSER_UNIFORM_H_