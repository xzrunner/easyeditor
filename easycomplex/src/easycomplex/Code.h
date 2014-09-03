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

	void ResolveUI(const Symbol& symbol);
	void ResolveText(const Symbol& symbol);

private:
	ebuilder::CodeGenerator& m_gen;

}; // Code

}

#endif // _EASYCOMPLEX_CODE_H_