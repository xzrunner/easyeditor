#ifndef _EASYCOMPLEX_CODE_H_
#define _EASYCOMPLEX_CODE_H_

#include <string>

namespace ebuilder { class CodeGenerator; }

namespace ecomplex 
{

class Symbol;

class Code
{
public:
	Code(ebuilder::CodeGenerator& gen);

	void Resolve(const Symbol& symbol);

private:
	struct Node
	{
		const Symbol& symbol;
		std::string path;
		std::string name;

		Node(const Symbol& _symbol, const std::string& _path) 
			: symbol(_symbol), path(_path) {}
	}; // Node

private:
	ebuilder::CodeGenerator& m_gen;

}; // Code

}

#endif // _EASYCOMPLEX_CODE_H_