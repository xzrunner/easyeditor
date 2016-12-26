#ifndef _SHADERLAB_PARSER_VARIABLE_H_
#define _SHADERLAB_PARSER_VARIABLE_H_

#include "VariableType.h"

#include <string>

namespace sl
{
namespace parser
{

class Variable
{
public:
	Variable(VariableType type) 
		: m_type(type) {}
	Variable(VariableType type, std::string name) 
		: m_type(type), m_name(name) {}
	Variable(const Variable& var)
		: m_type(var.m_type), m_name(var.m_name) {}
	virtual ~Variable() {}

	virtual std::string& ToStatement(std::string& str) const { return str; }

	VariableType GetType() const { return m_type; }
	const char* GetName() const { return m_name.c_str(); }

	void SetName(const std::string& name) { m_name = name; }

protected:
	VariableType m_type;
	std::string m_name;

}; // Variable

}
}

#endif // _SHADERLAB_PARSER_VARIABLE_H_