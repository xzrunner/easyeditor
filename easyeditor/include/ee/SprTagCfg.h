#ifndef _EASYEDITOR_SPR_TAG_CFG_H_
#define _EASYEDITOR_SPR_TAG_CFG_H_

#include <cu/cu_macro.h>

#include <vector>

namespace ee
{

class DynamicWidget;

class SprTagCfg
{
public:
	const std::vector<DynamicWidget*>& GetWidgets() const { return m_widgets; }
	
private:
	void LoadFromFile(const char* filename);

private:
	std::vector<DynamicWidget*> m_widgets;
	
	CU_SINGLETON_DECLARATION(SprTagCfg);

}; // SprTagCfg

}

#endif // _EASYEDITOR_SPR_TAG_CFG_H_