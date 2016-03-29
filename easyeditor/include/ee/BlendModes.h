#ifndef _EASYEDITOR_BLEND_MODES_H_
#define _EASYEDITOR_BLEND_MODES_H_

#include <vector>

namespace ee
{

enum BlendMode
{
	// normal
	BM_NULL				= 0,
	BM_DISSOLVE,		// todo

	// darken modes
	BM_DARKEN			= 10,
	BM_MULTIPLY,
	BM_COLOR_BURN,
	BM_LINEAR_BURN,
	BM_DARKER_COLOR,	// todo

	// lighten modes
	BM_LIGHTEN			= 20,
	BM_SCREEN,
	BM_COLOR_DODGE,
	BM_LINEAR_DODGE,
	BM_LIGHTER_COLOR,	// todo

	// saturation modes
	BM_OVERLAY			= 30,
	BM_SOFT_LIGHT,
	BM_HARD_LIGHT,
	BM_VIVID_LIGHT,
	BM_LINEAR_LIGHT,
	BM_PIN_LIGHT,
	BM_HARD_MIX,

	// substraction modes
	BM_DIFFERENCE		= 40,
	BM_EXCLUSION,

	// color modes 
	BM_HUE = 50,		// todo
	BM_SATURATION,		// todo
	BM_COLOR,			// todo
	BM_LUMINOSITY,		// todo
};

class BlendModes
{
public:
	BlendMode GetModeFromNameEN(const std::string& name) const;
	std::string GetNameENFromMode(BlendMode mode) const;

	BlendMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(BlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	static BlendModes* Instance();

private:
	BlendModes();

private:
	 struct Item
	 {
		Item(BlendMode mode, const std::string& name_en, const std::string& name_cn)
			: mode(mode), name_en(name_en), name_cn(name_cn) {}

	 	BlendMode mode;
	 	std::string name_en;
	 	std::string name_cn;
	 };

private:
	std::vector<Item> m_modes;

private:
	static BlendModes* m_instance;

}; // BlendModes

}

#endif // _EASYEDITOR_BLEND_MODES_H_