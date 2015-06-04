#ifndef _DRAG2D_BLEND_MODES_H_
#define _DRAG2D_BLEND_MODES_H_

#include <vector>
#include <wx/utils.h>

namespace d2d
{

enum BlendMode
{
	// normal
	BM_NORMAL = 0,
	BM_DISSOLVE,		// todo

	// darken modes
	BM_DARKEN = 10,
	BM_MULTIPLY,
	BM_COLOR_BURN,
	BM_LINEAR_BURN,
	BM_DARKER_COLOR,	// todo

	// lighten modes
	BM_LIGHTEN = 20,
	BM_SCREEN,
	BM_COLOR_DODGE,
	BM_LINEAR_DODGE,
	BM_LIGHTER_COLOR,	// todo

	// saturation modes
	BM_OVERLAY = 30,
	BM_SOFT_LIGHT,
	BM_HARD_LIGHT,
	BM_VIVID_LIGHT,
	BM_LINEAR_LIGHT,
	BM_PIN_LIGHT,
	BM_HARD_MIX,

	// substraction modes
	BM_DIFFERENCE = 40,
	BM_EXCLUSION,

	// color modes 
	BM_HUE = 50,		// todo
	BM_SATURATION,		// todo
	BM_COLOR,			// todo
	BM_LUMINOSITY,		// todo

	// others
	BM_UNKNOWN = 99,
};

class BlendModes
{
public:
	BlendMode GetIDFromNameEN(const std::string& name) const;
	std::string GetNameENFromID(BlendMode id) const;

	BlendMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(BlendMode mode) const;

	void GetAllNameCN(wxArrayString& names) const;

	static BlendModes* Instance();

private:
	BlendModes();

private:
	 struct Item
	 {
		Item(BlendMode id, const std::string& name_en, const std::string& name_cn)
			: id(id), name_en(name_en), name_cn(name_cn) {}

	 	BlendMode id;
	 	std::string name_en;
	 	std::string name_cn;
	 };

private:
	std::vector<Item> m_modes;

private:
	static BlendModes* m_instance;

}; // BlendModes

}

#endif // _DRAG2D_BLEND_MODES_H_