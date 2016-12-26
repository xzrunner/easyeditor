#include "AttributeNode.h"
#include "Attribute.h"

namespace sl
{
namespace parser
{

AttributeNode::AttributeNode(Variable var)
	: m_var(var)
{
	m_attributes.push_back(new Attribute(var));	
}

std::string& AttributeNode::ToStatements(std::string& str) const
{
	return str;
}

Variable AttributeNode::GetOutput() const
{
	return m_var;
}

}
}