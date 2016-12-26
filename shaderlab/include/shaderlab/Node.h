#ifndef _SHADERLAB_PARSER_NODE_H_
#define _SHADERLAB_PARSER_NODE_H_

#include "IOType.h"
#include "Variable.h"

#include <string>
#include <vector>

namespace sl
{
namespace parser
{

class Attribute;
class Varying;
class Uniform;

class Node
{
public:
	Node();
	virtual ~Node();

	virtual std::string& GetHeader(std::string& str) const { return str; }
	virtual std::string& ToStatements(std::string& str) const { return str; }

	virtual Variable GetOutput() const = 0;

	Node* Connect(Node* next);

	void GetVariables(IOType type, std::vector<const Variable*>& variables) const;

	const Node* Next() const { return m_output; }

protected:
	static void CheckType(const Variable& left, const Variable& right);
	static void CheckType(const Variable& var, VariableType type);

protected:
	Node *m_input, *m_output;

	std::vector<Attribute*> m_attributes;
	std::vector<Varying*> m_varyings;
	std::vector<Uniform*> m_uniforms;

	//std::string m_type;
	//std::string m_name;

}; // Node



}
}

#endif // _SHADERLAB_PARSER_NODE_H_