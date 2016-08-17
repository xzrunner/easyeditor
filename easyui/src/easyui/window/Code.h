#ifndef _EASYUI_WINDOW_CODE_H_
#define _EASYUI_WINDOW_CODE_H_

#include <vector>

namespace ebuilder { class CodeGenerator; }
namespace ee { class Sprite; }

namespace eui 
{
namespace window
{

class Code
{
public:
	Code(ebuilder::CodeGenerator& gen);

	void ResolveUI(const std::vector<ee::Sprite*>& sprs);
	void ResolveText(const std::vector<ee::Sprite*>& sprs);

private:
	ebuilder::CodeGenerator& m_gen;

}; // Code

}
}

#endif // _EASYUI_WINDOW_CODE_H_