#include "Node.h"
#include "Attribute.h"
#include "Varying.h"
#include "ParserUniform.h"

#include <stddef.h>
#include <stdio.h>

namespace sl
{
namespace parser
{

Node::Node()
	: m_input(NULL)
	, m_output(NULL)
{
}

Node::~Node()
{
	for (int i = 0, n = m_attributes.size(); i < n; ++i) {
		delete m_attributes[i];
	}
	for (int i = 0, n = m_varyings.size(); i < n; ++i) {
		delete m_varyings[i];
	}
	for (int i = 0, n = m_uniforms.size(); i < n; ++i) {
		delete m_uniforms[i];
	}
}

Node* Node::Connect(Node* next)
{
	m_output = next;
	next->m_input = this;
	return next;
}

void Node::GetVariables(IOType type, std::vector<const Variable*>& variables) const
{
	if (type == IOT_ATTRIBUTE) {
		std::copy(m_attributes.begin(), m_attributes.end(), back_inserter(variables));
	} else if (type == IOT_UNIFORM) {
		std::copy(m_uniforms.begin(), m_uniforms.end(), back_inserter(variables));
	} else if (type == IOT_VARYING) {
		std::copy(m_varyings.begin(), m_varyings.end(), back_inserter(variables));
	}
}

void Node::CheckType(const Variable& left, const Variable& right)
{
	if (left.GetType() != right.GetType()) {
		printf("assign type fail: %s = %s\n", left.GetName(), right.GetName());
		throw std::exception();
	}
}

void Node::CheckType(const Variable& var, VariableType type)
{
	if (var.GetType() != type) {
		printf("assign type fail: %s %d, %d\n", var.GetName(), var.GetType(), type);
		throw std::exception();
	}
}

}
}