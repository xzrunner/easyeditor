#ifndef _SHADERLAB_PARSER_FRAG_COLOR_H_
#define _SHADERLAB_PARSER_FRAG_COLOR_H_

#include "Node.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

class FragColor : public Node
{
public:
	virtual std::string& ToStatements(std::string& str) const {
		if (m_input) {
			char buf[128];
			sprintf(buf, "gl_FragColor = %s;\n", m_input->GetOutput().GetName());
			str += buf;
		}
		return str;
	}

	virtual Variable GetOutput() const { return Variable(VT_FLOAT4, "gl_FragColor"); }

}; // FragColor

}
}

#endif // _SHADERLAB_PARSER_FRAG_COLOR_H_