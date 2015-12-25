#ifndef _LR_UNIT_CFG_H_
#define _LR_UNIT_CFG_H_

#include <vector>

namespace lr
{

class DynamicWidget;

class UnitCfg
{
public:
	static UnitCfg* Instance();

private:
	UnitCfg();

	void LoadFromFile(const char* filename);

public:
	int col;

	std::vector<DynamicWidget*> widgets;

private:
	static UnitCfg* m_instance;

}; // UnitCfg

}

#endif // _LR_UNIT_CFG_H_