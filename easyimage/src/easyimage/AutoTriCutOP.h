#ifndef _EASYIMAGE_AUTO_TRI_CUT_OP_H_
#define _EASYIMAGE_AUTO_TRI_CUT_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

#include <vector>

namespace eimage
{

class AutoTriCutOP : public ee::ZoomViewOP
{
public:
	AutoTriCutOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnActive();

	virtual bool OnDraw() const;

private:
	std::vector<sm::vec2> m_raw_bound_line;
	std::vector<sm::vec2> m_raw_bound_line_merged;
	std::vector<sm::vec2> m_raw_bound_points;

	std::vector<sm::vec2> m_fine_bound_line;

	friend class AutoTriCutCMPT;

}; // AutoTriCutOP

}

#endif // _EASYIMAGE_AUTO_TRI_CUT_OP_H_